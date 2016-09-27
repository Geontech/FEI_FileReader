AC_INIT(FEI_FileReader, 1.0.0)
AM_INIT_AUTOMAKE([nostdinc foreign])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_CORBA_ORB
OSSIE_CHECK_OSSIE
OSSIE_SDRROOT_AS_PREFIX

AC_ARG_WITH(domainname,
        AS_HELP_STRING([--with-domainname=[domain name]], [Provide a domain name to the generated node, defaults to REDHAWK_DEV]),
        fei_filereader_node_domain=$withval,
        fei_filereader_node_domain=REDHAWK_DEV)
AC_SUBST(DOMAINNAME, $fei_filereader_node_domain)

AC_ARG_WITH(loop,
        AS_HELP_STRING([--with-loop=[boolean]], [Provide a loop value to the generated node, defaults to false]),
        fei_filereader_loop=$withval,
        fei_filereader_loop=false)
AC_SUBST(LOOP, $fei_filereader_loop)

AC_ARG_WITH(filepath,
        AS_HELP_STRING([--with-filepath=[path to file or directory]], [Provide a file path to the generated node, defaults to '']),
        fei_filereader_filepath=$withval,
        fei_filereader_filepath='')
AC_SUBST(FILEPATH, $fei_filereader_filepath)

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Dependencies
PKG_CHECK_MODULES([PROJECTDEPS], [ossie >= 2.0 omniORB4 >= 4.1.0])
PKG_CHECK_MODULES([INTERFACEDEPS], [frontend >= 2.2, bulkio >= 2.0])
OSSIE_ENABLE_LOG4CXX
AX_BOOST_BASE([1.41])
AX_BOOST_FILESYSTEM
AX_BOOST_IOSTREAMS
AX_BOOST_SYSTEM
AX_BOOST_THREAD
AX_BOOST_REGEX

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

