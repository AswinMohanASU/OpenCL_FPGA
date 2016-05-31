# tcsh
# License file
setenv LM_LICENSE_FILE 1800@maxwell.fulton.asu.edu
#setenv LM_LICENSE_FILE 1800@enlicense8.eas.asu.edu
echo \*\*\*\*WARNING\*\*\*\*: Note that the Altera tools require the environment variable \$LM_LICENSE_FILE to be set in order to access the license server. MATLAB will check our this variable too when being launched. This may cause the "parpool" call in MATLAB to stall. To run "parpool" smoothly in MATLAB, you should not source the Altera tool setup file or have \$LM_LICENSE_FILE unset. 

setenv ALTERAOCLSDKROOT "/home/tools/altera/14.1/hld"
setenv QUARTUS_ROOTDIR "/home/tools/altera/14.1/quartus"
setenv QSYS_ROOTDIR "/home/tools/altera/14.1/quartus/sopc_builder/bin"

setenv PATH ${PATH}:$QUARTUS_ROOTDIR/bin
#setenv PATH ${PATH}:/home/tools/altera/14.1/modelsim_ae/bin
setenv PATH ${PATH}:/home/tools/altera/14.1/nios2eds/bin

# Altera OpenCL SDK for DE5 Board
setenv AOCL_BOARD_PACKAGE_ROOT $ALTERAOCLSDKROOT/board/de5net
# DE5NET_Network
#setenv AOCL_BOARD_PACKAGE_ROOT $ALTERAOCLSDKROOT/board/de5net_network

setenv PATH ${PATH}:$ALTERAOCLSDKROOT/bin
setenv PATH ${PATH}:$ALTERAOCLSDKROOT/linux64/bin
setenv PATH ${PATH}:$AOCL_BOARD_PACKAGE_ROOT/linux64/lib
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:$ALTERAOCLSDKROOT/host/linux64/lib 
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:$AOCL_BOARD_PACKAGE_ROOT/linux64/lib
setenv QUARTUS_64BIT 1
#setenv CL_CONTEXT_EMULATOR_DEVICE_ALTERA de5net_a7  #Added for Emulator Environment
