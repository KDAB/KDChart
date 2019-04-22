from shutil import copyfile, rmtree
from string import Template
import os.path
import re
import sys
import autogen

class ForwardHeaderGenerator():
	def __init__( self, copy, path, includepath, srcpath, project, subprojects, prefix,
				prefixed, cleanIncludeDir = True, additionalHeaders = {} ):
		self.copy = copy
		self.path = path
		self.includepath = includepath
		self.srcpath = srcpath
		self.project = project
		self.subprojects = subprojects
		self.prefix = prefix
		self.prefixed = prefixed
		self.cleanIncludeDir = cleanIncludeDir
		self.additionalHeaders = additionalHeaders
		self.__projectFile = ""

	def run( self ):
		self.createProject()

	def _isValidHeaderFile( self, filename ):
		if ( filename.endswith( ".h" ) ):
			if filename.startswith( "moc_" ):
				return False
			if filename.startswith( "ui_" ):
				return False
			if filename.startswith( "qrc_" ):
				return False;
			if filename.endswith( "_p.h" ):
				return False
			return True
		else:
			return False

	def _suggestedHeaderNames( self, project, header ):
		regex = re.compile( "(?:class\s+[{0}|{1}][_0-9A-Z]*_EXPORT|MAKEINCLUDES_EXPORT)\s+([a-zA-Z_][A-Za-z0-9_]*)"
		                     .format( project.upper(), self.project.upper() ) )
		regex2 = re.compile( "(?:class\s+MAKEINCLUDES_EXPORT)\s+([a-zA-Z_][A-Za-z0-9_]*)" )
		regex3 = re.compile( "(?:\\\\file)\s+([a-zA-Z_][A-Za-z0-9_]*)" )

		f = open( header, "r" )
		classNames = set()

		for line in f.readlines():
			line = line.strip()

			className = None
			noPrefix = False
			if ( regex.match( line ) ):
				className = regex.match( line ).groups()[0]
				noPrefix = False
			else:
				if regex2.match( line ):
					className = regex2.match( line ).groups()[0]
					noPrefix = False
				else:
					if ( None != regex3.search( line ) ):
						className = regex3.search( line ).groups()[0]
						noPrefix = True

			if not className:
				continue

			if self.prefixed and not noPrefix:
				className = project + className

			classNames.add( className )

		f.close()

		return classNames

	def _addForwardHeader( self, targetPath, fileName, projectFile ):
		INCLUDE_STR = "#include \"{0}\""
		newHeader = open( targetPath, "wb" )
		newHeader.write( INCLUDE_STR.format( fileName ) + os.linesep )
		newHeader.close()

		basename = os.path.basename( targetPath )
		projectFile.write( basename + " \\" + os.linesep )

	def _createForwardHeader( self, header, projectFile, project ):
		path = os.path.dirname( header )
		basename = os.path.basename( header )
		classNames = self._suggestedHeaderNames( project, header )

		if len( classNames ) > 0:
			for classname in classNames:
				fHeaderName = os.path.abspath( path + "/" + classname )
				self._addForwardHeader( fHeaderName, basename, projectFile )

			projectFile.write( basename + " \\" + os.linesep )
		elif not basename in self.additionalHeaders.values(): # only create "foo" for "foo.h" if additionalHeaders doesn't overrides it
			sanitizedBasename = basename.replace( ".h", "" )

			#fHeaderName = os.path.abspath( self.includepath + "/" + sanitizedBasename )
			#self._addForwardHeader( fHeaderName, basename, self.__projectFile )

			fHeaderNameProjectDir = os.path.dirname( os.path.abspath( header ) ) + "/" + sanitizedBasename;
			self._addForwardHeader( fHeaderNameProjectDir, "{0}".format( basename ), projectFile )
			projectFile.write( basename + " \\" + os.linesep )

	def createProject( self ):
		if ( not os.path.exists( self.path ) ):
			errStr = Template( "Error, the directory $DIR does not exist!" )
			errStr = errStr.substitute( DIR = self.path )
			raise BaseException( errStr )

		if self.cleanIncludeDir and os.path.exists( self.includepath ):
			rmtree( self.includepath )
		if not os.path.exists( self.includepath ):
			os.mkdir( self.includepath )

		if autogen.policyVersion() >= 2:
			includeProjectName = os.path.basename( self.includepath.rstrip( "/" ) )
		else:
			includeProjectName = self.project
		profilename = os.path.abspath( self.includepath ) + "/" + includeProjectName + ".pro"
		projectFile = open( profilename, "wb" )
		self.__projectFile = projectFile
		lines = []
		lines.append( "TEMPLATE = subdirs" + os.linesep )
		lines.append( "SUBDIRS = " )

		for subProject in self.subprojects:
			line = subProject
			if ( subProject != self.subprojects[ -1 ] ):
				line += " \\"
			lines.append( line + os.linesep )

		projectFile.writelines( lines )
		projectFile.write( os.linesep )

		projectFile.write( "INSTALL_HEADERS.files = " )

		for subProject in self.subprojects:
			self._createSubproject( subProject )

		for fileName, includePath in self.additionalHeaders.items():
			targetPath = os.path.join( self.includepath, fileName )
			self._addForwardHeader( targetPath , includePath, projectFile )

		self._copyHeaders( self.srcpath, self.includepath, projectFile, self.project, self.prefixed )
		installPath = "{0}/include".format( self.prefix )
		self._projectFile_finalize( projectFile, installPath )
		projectFile.close()

	def _createSubproject( self, project ):
		inclPath = os.path.abspath( self.includepath + "/" + project )
		srcPath = os.path.abspath( self.srcpath + "/" + project )
		os.mkdir( inclPath )
		profilename = os.path.abspath( self.includepath ) + "/" + project + "/" + project + ".pro"
		projectFile = open( profilename, "wb" )
		projectFile.write( "TEMPLATE = subdirs" + os.linesep )
		projectFile.write( "INSTALL_HEADERS.files = " )
		self._copyHeaders( srcPath, inclPath, projectFile, project, self.prefixed )
		installPath = "{0}/include/{1}".format( self.prefix, project )
		self._projectFile_finalize( projectFile, installPath )
		projectFile.close()

	def _projectFile_finalize( self, projectFile, installPath ):
		projectFile.write( os.linesep )
		#projectFile.write( "message( $$INSTALL_HEADERS.path )" + os.linesep )
		projectFile.write( "INSTALL_HEADERS.path = {0}".format( installPath ) + os.linesep )
		#projectFile.write( "message( $$INSTALL_HEADERS.path )" + os.linesep )
		projectFile.write( "INSTALLS += INSTALL_HEADERS" + os.linesep )

	def _copyHeaders( self, srcDir, destDir, projectFile, project, prefixed = False ):
		rootDir = srcDir == self.srcpath
		dir = os.listdir( srcDir )
		headersForCatchAll = []
		for filename in dir:
			if ( rootDir ):
				if ( filename in self.subprojects ):
					continue
			file = os.path.abspath( srcDir + "/" + filename )
			if os.path.isdir( file ):
				self._copyHeaders( file, destDir, projectFile, project, prefixed )
			else:
				if self._isValidHeaderFile( filename ):
					destfile = os.path.abspath( destDir + "/" + filename )
					srcfile = os.path.abspath( srcDir + "/" + filename )
					copyfile( srcfile, destfile )
					self._createForwardHeader( destfile, projectFile, project )
					headersForCatchAll.append( filename )
		# Create "catch all" convenience header including all headers:
		if len( headersForCatchAll ) > 0:
			catchAllFileName = os.path.abspath( destDir + "/" + os.path.basename( destDir ) )
			catchAllContent = ["#include \"%s\"%s" % (header, os.linesep) for header in headersForCatchAll]
			catchAllFile = open( catchAllFileName, "wb" )
			catchAllFile.writelines( catchAllContent )
			catchAllFile.close()
			projectFile.write( os.path.basename( catchAllFileName ) + " \\" + os.linesep )

