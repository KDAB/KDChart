from datetime import datetime
import os.path

class CPackGenerateConfiguration():
	def __init__( self, projectName, version, sourceDirectory, buildDirectory, revision,
	              licenseFile, isTaggedRevision = False ):
		self._projectName = projectName
		self._sourceDirectory = sourceDirectory
		self._buildDirectory = buildDirectory
		self._revision = revision
		self._licenseFile = licenseFile
		self._isTaggedRevision = isTaggedRevision
		versionList = version.split( "." )
		assert( isinstance( versionList, list ) and len( versionList ) == 3 )
		self._versionList = versionList
		self._generators = { 'WINDOWS': 'NSIS', 'APPLE': 'ZIP', 'ELSE': 'TBZ2' }

	def fixCMakeWindowsPaths( self, path ):
		return path.replace( '\\', '\\\\' )

	def ignoreString( self ):
		ignores = str()
		try:
			with open( os.path.join( self._sourceDirectory, 'CPackIgnores.txt' ) ) as ignoresFile:
				for ignore in ignoresFile:
					# using ${CPACK_INSTALL_DIRECTORY} or similar seems to be the only way to specify
					# "absolute" *sub*directory names. XPLATFORM_INSTALL_DIR is set by us and like
					# CPACK_INSTALL_DIRECTORY, but with forward slashes instead of backslashes.
					ignores += '\n                       "^${XPLATFORM_INSTALL_DIR}' + ignore[ :-1 ] + '"'
		except:
			pass # It's not mandatory to add ignores
		return ignores

	def _formattedConfiguration( self ):
		with open(os.path.dirname(__file__) + '/cpack.cmake.in') as configFile:
			config = configFile.read()

		# Can't do this with str.format because of CMake's variable escaping conflicting
		# with Python's format escaping
		packageName = self._projectName
		packageNameSimplified = packageName.lower().replace( ' ', '_' )
		config = config.replace( "@CPACK_PACKAGE_NAME@", packageName, 1 )
		config = config.replace( "@CPACK_PACKAGE_NAME_SIMPLIFIED@", packageNameSimplified, 1 )

		versionList = self._versionList
		config = config.replace( "@CPACK_PACKAGE_VERSION_MAJOR@", versionList[0] or 1, 1 )
		config = config.replace( "@CPACK_PACKAGE_VERSION_MINOR@", versionList[1] or 0, 1 )
		patchVersion = versionList[2] or 0
		if not self._isTaggedRevision:
			patchVersion += '-r' + self._revision
		config = config.replace( "@CPACK_PACKAGE_VERSION_PATCH@", patchVersion, 1 )
		installDirectory = self.fixCMakeWindowsPaths( self._sourceDirectory )
		config = config.replace( "@CPACK_INSTALL_DIRECTORY@", installDirectory, 1 )
		config = config.replace( "@CPACK_EXTRA_IGNORE_FILES@", self.ignoreString(), 1 )

		licenseFile = self._licenseFile

		if not licenseFile:
			licenseFile = os.path.join( self._buildDirectory, "CPackGeneratedLicense.txt" )
			with open( licenseFile, 'w' ) as license:
				license.write( '{0} - Copyright {1}, All Rights Reserved.'.format( packageName, datetime.now().year ) )
		else:
			licenseFile = os.path.abspath( licenseFile ) # NSIS apparently requires an absolute path to find the license file
		licenseFile = self.fixCMakeWindowsPaths( licenseFile )

		config = config.replace( "@CPACK_RESOURCE_FILE_LICENSE@", licenseFile )

		for platform in ( 'WINDOWS', 'APPLE', 'ELSE' ):
			generator = self._generators[ platform ]
			config = config.replace( "@CPACK_GENERATOR_%s@" % platform, generator )

		return config

	def run( self ):
		config = os.path.join( self._buildDirectory, 'CPackConfig.cmake' )
		with open( config, 'w' ) as configFile:
			configFile.write( self._formattedConfiguration() )
