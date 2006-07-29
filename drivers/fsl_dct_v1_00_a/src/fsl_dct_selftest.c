//////////////////////////////////////////////////////////////////////////////
// Filename:          D:\mb-jpeg\drivers/fsl_dct_v1_00_a/src/fsl_dct_selftest.c
// Version:           1.00.a
// Description:       
// Date:              Fri Jul 21 08:31:12 2006 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////

#include "xparameters.h"
#include "fsl_dct.h"

/* IMPORTANT:
*  In order to run this self test, you need to modify the value of following
*  micros according to the slot ID defined in xparameters.h file. 
*/
#define input_slot_id   XPAR_FSL_FSL_DCT_0_INPUT_SLOT_ID
#define output_slot_id  XPAR_FSL_FSL_DCT_0_OUTPUT_SLOT_ID

XStatus FSL_DCT_SelfTest()
{
	 unsigned int input_0[8];     
	 unsigned int output_0[8];     

	 //Initialize your input data over here: 
	 input_0[0] = 12345;     
	 input_0[1] = 24690;     
	 input_0[2] = 37035;     
	 input_0[3] = 49380;     
	 input_0[4] = 61725;     
	 input_0[5] = 74070;     
	 input_0[6] = 86415;     
	 input_0[7] = 98760;     

	 //Call the macro with instance specific slot IDs
	 fsl_dct(
		 input_slot_id,
		 output_slot_id,
		 input_0,      
		 output_0       
		 );

	 if (output_0[0] != 444420)
		 return XST_FAILURE;
	 if (output_0[7] != 444420)
		 return XST_FAILURE;

	 return XST_SUCCESS;
}
