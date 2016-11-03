#!/usr/bin/env python

import os, sys, commands, logging, platform, shutil, socket
from ossie import parsers
from ossie.utils.model import _uuidgen as uuidgen

class ConfigurationError(StandardError):
    pass

class NodeConfig(object):
    def __init__(self, options, cmdlineProps):
        # Basic setup
        self._log = logging.getLogger('NodeConfig')
        self.localfile_nodeprefix = '/mgr'
        self.options = options
        self.cmdlineProps = cmdlineProps
        self.hostname = socket.gethostname()
        
        # check domainname
        if options.domainname == None:
            raise ConfigurationError("A domainname is required")
        
        # Verify the base FEI_FileReader profile exists
        self.fei_filereader_templates = {"spd": os.path.join(self.options.sdrroot, "dev", self.options.feifilereaderpath[1:], "FEI_FileReader.spd.xml"),
                                         "prf": os.path.join(self.options.sdrroot, "dev", self.options.feifilereaderpath[1:], "FEI_FileReader.prf.xml"),
                                         "scd": os.path.join(self.options.sdrroot, "dev", self.options.feifilereaderpath[1:], "FEI_FileReader.scd.xml")}

        for template in self.fei_filereader_templates.values():
            if not os.path.exists(template):
                raise ConfigurationError("%s missing" % template)
                
        self.nodedir = os.path.join(self.options.sdrroot, "dev", "nodes", self.options.nodename.replace('.','/'))
        self.path_to_dcd = os.path.join(self.nodedir , "DeviceManager.dcd.xml")
            
        # Figure out where we are going to write the FEI_FileReader profile
        if self.options.inplace:
            self.fei_filereader_path = os.path.join(self.options.sdrroot, "dev", "devices", "FEI_FileReader")
        else:
            self.fei_filereader_path = os.path.join(self.nodedir, "FEI_FileReader")
            
        # prep uuids
        self.uuids = {}
        self.uuids["softpkg"                ] = 'DCE:' + uuidgen()
        self.uuids["implementation"         ] = 'DCE:' + uuidgen()
        self.uuids["deviceconfiguration"    ] = 'DCE:' + uuidgen()
        self.uuids["componentfile"          ] = 'DCE:' + uuidgen()
        self.uuids["componentinstantiation" ] = 'DCE:' + uuidgen()
        self.uuids["componentimplementation"] = 'DCE:' + uuidgen()
        self.uuids["componentsoftpkg"       ] = 'DCE:' + uuidgen()
        
        self.props = {}

    def register(self):
        if not self.options.silent:
            self._log.debug("Registering...")
        self._gather_fei_filereader_information()
        self._createDeviceManagerProfile()
        self._updateFEI_FileReaderProfile()
    
    def unregister(self):
        if not self.options.silent:
            self._log.debug("Unregistering...")
        if os.path.isdir(self.nodedir):
            if not self.options.silent:
                self._log.debug("  Removing <" + self.nodedir + ">")
            shutil.rmtree(self.nodedir)
         
    def _ver2rel(self, ver):
        return float(ver[0:1]) + float(ver[2:3])*0.1 + float(ver[4:5])*0.000001

    def _gather_fei_filereader_information(self):
        if not self.options.silent:
            self._log.debug("Checking FEI_FileReader capacity...")

        self.props["filePath"] = self.options.filepath
        self.props["loop"] = self.options.loop

    def _createDeviceManagerProfile(self):
        #####################
        # Setup environment
        #####################

        # make sure node hasn't already been created
        if os.path.exists(self.path_to_dcd):
            self._log.error("Cannot 'register' new dynamicnode. A previous configuration was found. Please 'unregister' dynamicnode first.")
            sys.exit(1)

        try:
            if not os.path.isdir(self.nodedir):
                os.makedirs(self.nodedir)
            else:
                if not self.options.silent:
                    self._log.debug("Node directory already exists; skipping directory creation")
                pass
        except OSError:
            raise Exception, "Could not create device manager directory"

        FEI_FileReader_componentfile = 'FEI_FileReader__' + uuidgen()
        if self.options.inplace:
            compfiles = [{'id':FEI_FileReader_componentfile, 'localfile':os.path.join('/devices', 'FEI_FileReader', 'FEI_FileReader.spd.xml')}]
        else:
            compfiles = [{'id':FEI_FileReader_componentfile, 'localfile':os.path.join('/nodes', self.options.nodename.replace('.','/'), 'FEI_FileReader', 'FEI_FileReader.spd.xml')}]
        compplacements = [{'refid':FEI_FileReader_componentfile, 'instantiations':[{'id':self.uuids["componentinstantiation"], 'usagename':'FEI_FileReader_' + self.hostname.replace('.', '_')}]}]

        #####################
        # DeviceManager files
        #####################
        if not self.options.silent:
            self._log.debug("Creating DeviceManager profile <" + self.options.nodename + ">")
        
        # set deviceconfiguration info
        _dcd = parsers.DCDParser.deviceconfiguration()
        _dcd.set_id(self.uuids["deviceconfiguration"])
        _dcd.set_name(self.options.nodename)
        _localfile = parsers.DCDParser.localfile(name=os.path.join(self.localfile_nodeprefix, 'DeviceManager.spd.xml'))
        _dcd.devicemanagersoftpkg = parsers.DCDParser.devicemanagersoftpkg(localfile=_localfile)
        
        # add componentfiles and componentfile(s)
        _dcd.componentfiles = parsers.DCDParser.componentfiles()
        for in_cf in compfiles:
            cf = parsers.DCDParser.componentfile(type_='SPD', id_=in_cf['id'], localfile=parsers.DCDParser.localfile(name=in_cf['localfile']))
            _dcd.componentfiles.add_componentfile(cf)

        # add partitioning/componentplacements
        _dcd.partitioning = parsers.DCDParser.partitioning()
        for in_cp in compplacements:
            _comp_fileref = parsers.DCDParser.componentfileref(refid=in_cp['refid'])
            _comp_placement = parsers.DCDParser.componentplacement(componentfileref=_comp_fileref)
            for ci in in_cp['instantiations']:
                comp_inst = parsers.DCDParser.componentinstantiation(id_=ci['id'], usagename=ci['usagename'])
                _comp_placement.add_componentinstantiation(comp_inst)
            _dcd.partitioning.add_componentplacement(_comp_placement)

        # add domainmanager lookup
        if self.options.domainname:
            _tmpdomainname = self.options.domainname + '/' + self.options.domainname
            
        _dcd.domainmanager = parsers.DCDParser.domainmanager(namingservice=parsers.DCDParser.namingservice(name=_tmpdomainname))
        dcd_out = open(self.path_to_dcd, 'w')
        dcd_out.write(parsers.parserconfig.getVersionXML())
        _dcd.export(dcd_out,0)
        dcd_out.close()
        
    def _updateFEI_FileReaderProfile(self):
        #####################
        # FEI_FileReader files
        #####################
        
        if not self.options.silent:
            self._log.debug("Creating FEI_FileReader profile <" + self.fei_filereader_path + ">")
            
        if not self.options.inplace:
            if not os.path.exists(self.fei_filereader_path):
                os.mkdir(self.fei_filereader_path)
            for f in self.fei_filereader_templates.values():
                shutil.copy(f, self.fei_filereader_path)
                
        self._updateFEI_FileReaderSpd()
        self._updateFEI_FileReaderPrf()
    
    def _updateFEI_FileReaderSpd(self):
        # update the spd file
        spdpath = os.path.join(self.fei_filereader_path, 'FEI_FileReader.spd.xml')
        _spd = parsers.SPDParser.parse(spdpath)
        _spd.set_id(self.uuids["componentsoftpkg"])
        _spd.implementation[0].set_id(self.uuids["componentimplementation"])

        # update the FEI_FileReader code entry if this wasn't an inplace update
        if not self.options.inplace:
            code = _spd.get_implementation()[0].get_code()
            new_entrypoint = os.path.normpath(os.path.join(self.options.feifilereaderpath, code.get_entrypoint()))
            new_localfile = os.path.normpath(os.path.join(self.options.feifilereaderpath, code.get_localfile().get_name()))
            code.set_entrypoint(new_entrypoint)
            code.get_localfile().set_name(new_localfile)
            
        spd_out = open(spdpath, 'w')
        spd_out.write(parsers.parserconfig.getVersionXML())
        _spd.export(spd_out,0, name_='softpkg')
        spd_out.close()
        
    def _updateFEI_FileReaderPrf(self):
        # generate the prf file
        prfpath = os.path.join(self.fei_filereader_path, 'FEI_FileReader.prf.xml')
        _prf = parsers.PRFParser.parse(prfpath)
        
        # Set the parameters for the device
        for simple in _prf.get_simple():
            if simple.get_id() in "loop":
                simple.set_value(str(self.props["loop"]))
            elif simple.get_id() in "filePath":
                simple.set_value(str(self.props["filePath"]))
            elif simple.get_id() in self.cmdlineProps.keys():
                simple.set_value(str(self.cmdlineProps[simple.get_id()]))

        prf_out = open(prfpath, 'w')
        prf_out.write(parsers.parserconfig.getVersionXML())
        _prf.export(prf_out,0)
        prf_out.close()
        

        
###########################
# Run from command line
###########################
if __name__ == "__main__":

    ##################
    # setup arg parser
    ##################
    from optparse import OptionParser
    parser = OptionParser()
    parser.usage = "%s [options] [simple_prop1 simple_value1]..."
    parser.add_option("--domainname", dest="domainname", default=None,
                      help="Must give a domainname")
    parser.add_option("--sdrroot", dest="sdrroot", default=os.path.expandvars("${SDRROOT}"),
                      help="Path to the sdrroot; if none is given, ${SDRROOT} is used.")
    parser.add_option("--nodename", dest="nodename", default="DevMgr_File_%s" % socket.gethostname(),
                      help="Desired nodename, if none is given DevMgr_File_${HOST} is used")
    parser.add_option("--inplace", dest="inplace", default=False, action="store_true",
                      help="Update the FEI_FileReader profile in-place; default is to create a FEI_FileReader configuration in the node folder")
    parser.add_option("--feifilereaderpath", dest="feifilereaderpath", default="/devices/FEI_FileReader",
                      help="The device manager file system absolute path to the FEI_FileReader, default '/devices/FEI_FileReader'")
    parser.add_option("--loop", dest="loop", default=False,
                      help="Set the loop property of the FEI_FileReader, default 'False'")
    parser.add_option("--filepath", dest="filepath", default="",
                      help="Set the filePath property of the FEI_FileReader, default ''")
    parser.add_option("--silent", dest="silent", default=False, action="store_true",
                      help="Suppress all logging except errors")
    parser.add_option("--clean", dest="clean", default=False, action="store_true",
                      help="Clean up the previous configuration for this node first (delete entire node)")
    parser.add_option("-v", "--verbose", dest="verbose", default=False, action="store_true",
                      help="Enable verbose logging")

    (options, args) = parser.parse_args()

    # Configure logging
    logging.basicConfig(format='%(name)-12s:%(levelname)-8s: %(message)s', level=logging.INFO)
    if options.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    # grab tmp logger until class is created
    _log = logging.getLogger('NodeConfig')

    if len(args) % 2 == 1:
        _log.error("Invalid command line arguments - properties must be specified with values")
        sys.exit(1)
    cmdlineProps = {}
    for i in range(len(args)):
        if i % 2 == 0:
            cmdlineProps[args[i]] = args[i + 1]

    # create instance of NodeConfig
    try:
        dn = NodeConfig(options, cmdlineProps)
        if options.clean:
            dn.unregister()
        dn.register()
        if not options.silent:
            _log.info("FEI_FileReader node registration is complete")
    except ConfigurationError, e:
        _log.error("%s", e)
        sys.exit(1)
