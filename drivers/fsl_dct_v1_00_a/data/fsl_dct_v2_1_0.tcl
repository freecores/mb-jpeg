##############################################################################
## Filename:          D:\mb-jpeg\drivers/fsl_dct_v1_00_a/data/fsl_dct_v2_1_0.tcl
## Description:       Tool Command Language
## Date:              Fri Jul 21 08:31:12 2006 (by Create and Import Peripheral Wizard)
##############################################################################

## Note:
## This tcl file will detect the FSL id number of the connected FSL interface,
## and define them as macro in xparameters.h file.

proc generate {drv_handle} {
   puts "Generating Macros for FSL peripheral access ..."
    set hw_inst_handle [xget_sw_parameter_handle $drv_handle "HW_INSTANCE"] 
    set hw_inst [xget_value $hw_inst_handle "value"]
    set drv_name_handle [xget_sw_parameter_handle $drv_handle "DRIVER_NAME"]
    set ipname [xget_value $drv_name_handle "value"]
    set header_file [format "x%s_fsl.h" $ipname]
    fsl_defines "xparameters.h" $hw_inst $ipname
}

proc fsl_defines {file_name core_name ipname} {
  set core_def_name [string toupper $core_name]
  if {[string compare -nocase "none" $core_name] != 0} {
     set conffile  [xopen_include_file $file_name]
     set sw_prochandle [xget_libgen_proc_handle]
     set ip_handle [xget_sw_ipinst_handle_from_processor $sw_prochandle $core_name]
     set mhs_handle [xget_handle $ip_handle "parent"]

     set mfsl_name [xget_value $ip_handle "BUS_INTERFACE" "MFSL"]
     if {$mfsl_name != ""} {
        set mfsl_slave [xget_hw_connected_busifs_handle $mhs_handle $mfsl_name "slave"]
        set mfsl_index [xget_value $mfsl_slave "NAME"]
        set mfsl_index [string toupper $mfsl_index]
        set mfsl_index [string map {SFSL ""} $mfsl_index]
        puts  $conffile "#define XPAR_FSL_${core_def_name}_OUTPUT_SLOT_ID  ${mfsl_index}"
     }

     set sfsl_name [xget_value $ip_handle "BUS_INTERFACE" "SFSL"]
     if {$sfsl_name != ""} {
        set sfsl_master [xget_hw_connected_busifs_handle $mhs_handle $sfsl_name "master"]
        set sfsl_index [xget_value $sfsl_master "NAME"]
        set sfsl_index [string toupper $sfsl_index]
        set sfsl_index [string map {MFSL ""} $sfsl_index]
        puts  $conffile "#define XPAR_FSL_${core_def_name}_INPUT_SLOT_ID  ${sfsl_index}"
     }

     puts  $conffile ""
     puts  $conffile "/******************************************************************/"
     puts  $conffile ""
     close $conffile
    } 
}

