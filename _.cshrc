##############
# Tool Setup #
##############

#Synopsys tools
source /home/tools/synopsys/Synopsys_Setup.csh
echo Synopsys tool setup is completed.

# Xilinx tools
source /home/tools/xilinx/Xilinx_Setup.csh
echo Xilinx tool setup is completed.

#Altera
source /home/user/agunavel/Backup/altera/Altera_setup_14.1_de5net.csh
echo Altera setup is completed.

# Mentor Graphics tools
source /home/tools/mentor/Mentor_setup.csh
echo Mentor Graphics tool setup is completed.

# Matlab
setenv PATH ${PATH}:/home/tools/matlab/R2014b/bin
echo MATLAB setup is completed.

setenv ALTERAOCLSDKROOT "/home/tools/altera/14.1/hld"

setenv QSYS_ROOTDIR "/home/tools/altera/14.1/quartus/sopc_builder/bin"



