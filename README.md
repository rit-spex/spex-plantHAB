# spex-plantHAB
NDVI High Altitude Balloon Payload

## Building
### Requirements:
- `libopencv-dev` and its dependencies

There are two different builds for the HAB-CV NDVI testing program
1. Workstation Build
    This build is for testing specific test cases in the form of image files
2. Integration Build
    This build is for testing the HAB-CV software on the actual HAB environment
    Test cases will be automatically generated using the attached camera (which must be present for successful execution of the program)

### Building Instructions for Workstations
To build this version of the software, use
```make all```
The default test may then be run using
```make testfile```
To test specific images, see the section on make variables below

### Building Instructions for Integration Environment
To build this version of the software, use
```make all DFLAGS=```
The test may then be run using
```make test```

### Make variables
These variables can be set from the command line to change the way the program is built or run. This is not an exhaustive list of make variables, but these are the only ones which should ever need to be changed through the command line.
This is an example of setting the TESTPICTURE variable to ndvi-test2.png:
```make testfile TESTFILE=ndvi-test2.png```
This is an example of clearing the DFLAGS variable (useful in building integration build):
```make all DFLAGS=```
### Variables
- DFLAGS
    - Used to set -D flags for the compiler
    - The default value of this is `-DNPI`
    - This variable only affects make rules which depend on the executable
- TESTPICTURE
    - Used to set the test picture in workstation build tests
    - The default value of this is `ndvi-test.png` (provided)
    - This variable only affects the `testfile` rule
- NOPV
    - Specifies whether or not to preview the output of the workstation test automatically
    - The default value of this is `-n`
        - This will disable the preview. To enable it, clear this variable when running `make testfile`
    - This variables only affects the `testfile` rule and only applies to the workstation build
- OUTPUTPATH
    - Specifies the path to store the result images
    - The default value of this is `output`
    - This rule affects both `test` and `testfile` as well as both builds
    - Using special path symbols here should be avoided as they are not all platform independant and are not all supported
        - `OUTPUT=~/output` might not work
        - `OUTPUT=/home/<username>/output` will always work
    - Placing a trailing `/` at the end of the path should also be avoided, but is unlikely to cause issues
- OUTPUTFILE
    - Specifies the log file for `test` and `testfile`
    - The default value of this is `<executable-name>.log`
- BUILDLOG
    - Specifies the log file for the build
    - The default value of this is `<executable-name-build>.log`
