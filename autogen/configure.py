import os.path

class ConfigureScriptGenerator():
	def __init__( self, project, path, version, install = True, static = True ):
		self.__project = project
		self.__path = path
		self.__version = version
		self.__staticSupported = static
		autogen_dir = os.path.dirname( __file__ )
		self.__winTemplate = os.path.abspath( autogen_dir + "/configure.bat.in" )
		self.__unixTemplate = os.path.abspath( autogen_dir + "/configure.sh.in" )

	def run( self ):
		self.__generateFile( self.__unixTemplate, os.path.abspath( self.__path + "/configure.sh" ), "unix" )
		self.__generateFile( self.__winTemplate, os.path.abspath( self.__path + "/configure.bat" ), "win32" )

	def __replaceValues( self, value ):
		mixedname = self.__project
		mixedname = mixedname.replace( "KD", "KD " )
		value = value.replace( "@VERSION@", self.__version )
		strStaticSupported = 'false'
		if self.__staticSupported:
			strStaticSupported = 'true'
		value = value.replace( "@STATIC_BUILD_SUPPORTED@", strStaticSupported )
		value = value.replace( "@PRODUCT_UPPERCASE@", self.__project.upper() )
		value = value.replace( "@PRODUCT_LOWERCASE@", self.__project.lower() )
		value = value.replace( "@PRODUCT_MIXEDCASE@", self.__project )
		value = value.replace( "@PRODUCT_MIXEDCASE_SPACED@", mixedname )
		return value

	def __generateFile( self, templateFile, outputFile, platformString ):
		if platformString == "win32":
			lineSep = "\r\n"
		else:
			lineSep = "\n"

		with open( outputFile, "wb" ) as fOutput:
			with open(os.path.dirname(__file__) + '/' + os.path.basename(outputFile) + '.in') as configureFile:
				configure = configureFile.read().splitlines()
			for line in ( configure ):
				fOutput.write( self.__replaceValues( line.rstrip() ) + lineSep )

		# make file executable for Unix
		if platformString != "win32":
			try:
				os.chmod( outputFile, 0755 )
			except SyntaxError: # Python 2.6 says syntax error on Windows, ignore it
				print "ignoring failing os.chmod call, configure.sh won't be executable"
