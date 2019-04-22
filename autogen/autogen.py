import os, sys, subprocess

if sys.version_info[0] != 2:
  print ("Autogen detected that you are not using Python2.")
  print ("Please make sure you have Python2 installed and run:")
  print ("  python2 autogen.py")
  sys.exit(0)

from cpack import CPackGenerateConfiguration 
from configure import ConfigureScriptGenerator
from header import ForwardHeaderGenerator

def parseSvnInfo( stdout ):
	lines = stdout.splitlines()
	repositoryUrl = [line for line in lines if line.startswith('URL:')][0].split( ':', 1 )[ 1 ]
	isTagged = repositoryUrl.find( 'tags/' ) != -1
	repositoryRevision = [line for line in lines if line.startswith('Revision:')][0].split( ':', 1 )[ 1 ].strip()
	return isTagged, repositoryRevision

def checkVCS( sourceDirectory ):
	repositoryType = ''
	for i in xrange( 20 ):
		listing = os.listdir( sourceDirectory + '/..' * i )
		if '.svn' in listing:
			repositoryType = 'svn'
			break
		elif '.git' in listing:
			repositoryType = 'git'
			break

	isTagged = False
	repositoryRevision = "unknown"
	try:
		if repositoryType == 'git':
			p = subprocess.Popen( ["git", "describe", "--exact-match"], cwd = sourceDirectory, stdout = subprocess.PIPE, stderr = subprocess.PIPE )
			stdout, stderr = p.communicate()
			if p.returncode == 0:
				# git tag
				isTagged = True
				repositoryRevision = stdout.strip()
			if p.returncode != 0:
				p = subprocess.Popen( ["git", "svn", "info"], cwd = sourceDirectory, stdout = subprocess.PIPE, stderr = subprocess.PIPE )
				stdout, stderr = p.communicate()
				if p.returncode == 0:
					 # git-svn revision
					isTagged, repositoryRevision = parseSvnInfo( stdout )
			if p.returncode != 0:
				p = subprocess.Popen( ["git", "rev-parse", "HEAD"], cwd = sourceDirectory, stdout = subprocess.PIPE, stderr = subprocess.PIPE )
				stdout, stderr = p.communicate()
				if p.returncode == 0:
					# svn revision
					repositoryRevision = stdout.strip()[:8]
		elif repositoryType == 'svn':
			p = subprocess.Popen( ["svn", "info"], cwd = sourceDirectory, stdout = subprocess.PIPE, stderr = subprocess.PIPE )
			stdout, stderr = p.communicate()
			if p.returncode == 0:
				isTagged, repositoryRevision = parseSvnInfo( stdout )
		else:
			raise Exception("Unknown repository type")
	except:
		print( "Error: Not a valid SVN or Git repository: {0}".format( sourceDirectory ) )
		sys.exit( 1 )

	return ( repositoryRevision, isTagged )

def autogen(project, version, subprojects, prefixed, forwardHeaderMap = {}, steps=["generate-cpack", "generate-configure", "generate-forward-headers"], installPrefix="$$INSTALL_PREFIX", policyVersion = 1):
	global __policyVersion
	__policyVersion = policyVersion
	sourceDirectory = os.path.abspath( os.path.dirname( os.path.dirname( __file__ ) ) )
	buildDirectory = os.getcwd()

	print( "-- Using source directory: {0}".format( sourceDirectory ) )

	( repositoryRevision, isTagged ) = checkVCS( sourceDirectory )

	print( "-- Using repository information: revision={0} isTagged={1}".format( repositoryRevision, isTagged ) )

	if "generate-cpack" in steps:
		licensePath = os.path.join( sourceDirectory, "LICENSE.txt" )
		cpackConfigurationGenerator = CPackGenerateConfiguration( project, version, sourceDirectory,
								    buildDirectory, repositoryRevision, licensePath, isTaggedRevision = isTagged )
		cpackConfigurationGenerator.run()

	if "generate-configure" in steps:
		configureScriptGenerator = ConfigureScriptGenerator( project, sourceDirectory, version )
		configureScriptGenerator.run()

	includePath = os.path.join( sourceDirectory, "include" )
	srcPath = os.path.join( sourceDirectory, "src" )

	if subprojects and "generate-cpack" in steps:
		forwardHeaderGenerator = ForwardHeaderGenerator( 
			copy = True, path = sourceDirectory, includepath = includePath, srcpath = srcPath,
			project = project, subprojects = subprojects, prefix = installPrefix, prefixed = prefixed,
			additionalHeaders = forwardHeaderMap			
		)
		forwardHeaderGenerator.run()

	print( "-- Auto-generation done." )

	with file( ".license.accepted", 'a' ):
		os.utime( ".license.accepted", None )
	print( "-- License marked as accepted." )

	print( "-- Wrote build files to: {0}".format( buildDirectory ) )
	print( "-- Now running configure script." )
	print( "" )
	sys.stdout.flush()

	configureFile = 'configure.bat' if sys.platform == 'win32' else 'configure.sh'
	configurePath = os.path.join( sourceDirectory, configureFile )
	os.execvp( configurePath, [configurePath] + sys.argv[1:] )

def policyVersion():
	global __policyVersion
	return __policyVersion
