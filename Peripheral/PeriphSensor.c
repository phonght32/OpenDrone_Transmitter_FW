#include "Periph.h"
#include "OpenDrone_Transmitter_Define.h"
#include "OpenDrone_Transmitter_HwIntf.h"
#include "joystick.h"

#ifdef USE_JOYSTICK_MODULE

joystick_handle_t left_joystick_handle, right_joystick_handle;
#endif

err_code_t PeriphSensor_Init(void)
{
#ifdef USE_JOYSTICK_MODULE
	left_joystick_handle = joystick_init();
	joystick_cfg_t left_joystick_cfg = {
		.min_acceptable_raw_x 	= 500,
		.max_acceptable_raw_x 	= 2508,
		.min_scale_x 			= 0,
		.max_scale_x 			= 2000,
		.inverse_x 				= 0,
		.min_acceptable_raw_y 	= 1090,
		.max_acceptable_raw_y 	= 2790,
		.min_scale_y 			= 0,
		.max_scale_y 			= 2000,
		.inverse_y 				= 1,
		.get_pos_x 				= hw_intf_left_joystick_get_pos_x,
		.get_pos_y 				= hw_intf_left_joystick_get_pos_y,
		.get_button 			= hw_intf_left_joystick_get_bt_status,
	};
	joystick_set_config(left_joystick_handle, left_joystick_cfg);
	joystick_config(left_joystick_handle);

	right_joystick_handle = joystick_init();
	joystick_cfg_t right_joystick_cfg = {
		.min_acceptable_raw_x 	= 1568,
		.max_acceptable_raw_x 	= 3510,
		.min_scale_x 			= 0,
		.max_scale_x 			= 2000,
		.inverse_x 				= 1,
		.min_acceptable_raw_y 	= 1270,
		.max_acceptable_raw_y 	= 2790,
		.min_scale_y 			= 0,
		.max_scale_y 			= 2000,
		.inverse_y 				= 0,
		.get_pos_x 				= hw_intf_right_joystick_get_pos_x,
		.get_pos_y 				= hw_intf_right_joystick_get_pos_y,
		.get_button 			= hw_intf_right_joystick_get_bt_status,
	};
	joystick_set_config(right_joystick_handle, right_joystick_cfg);
	joystick_config(right_joystick_handle);
#endif

	return ERR_CODE_SUCCESS;
}

err_code_t PeriphSensor_GetJoystickData(periph_operator_data_t *data)
{
	int64_t sum_left_joystick_x = 0, sum_left_joystick_y = 0;
	int64_t sum_right_joystick_x = 0, sum_right_joystick_y = 0;
	int64_t left_joystick_x = 0, left_joystick_y = 0;
	int64_t right_joystick_x = 0, right_joystick_y = 0;
	uint8_t num_conversion = 20;
	int16_t temp_data = 0;
	uint8_t i = 0;

	for (i = 0; i < num_conversion; i++)
	{
		joystick_get_pos_x_scale(left_joystick_handle, &temp_data);
		sum_left_joystick_x += temp_data;
	}
	left_joystick_x = sum_left_joystick_x / num_conversion;

	for (i = 0; i < num_conversion; i++)
	{
		joystick_get_pos_y_scale(left_joystick_handle, &temp_data);
		sum_left_joystick_y += temp_data;
	}
	left_joystick_y = sum_left_joystick_y / num_conversion;

	for (i = 0; i < num_conversion; i++)
	{
		joystick_get_pos_x_scale(right_joystick_handle, &temp_data);
		sum_right_joystick_x += temp_data;
	}
	right_joystick_x = sum_right_joystick_x / num_conversion;

	for (i = 0; i < num_conversion; i++)
	{
		joystick_get_pos_y_scale(right_joystick_handle, &temp_data);
		sum_right_joystick_y += temp_data;
	}
	right_joystick_y = sum_right_joystick_y / num_conversion;

	data->left_joystick_x = left_joystick_x;
	data->left_joystick_y = left_joystick_y;
	data->right_joystick_x = right_joystick_x;
	data->right_joystick_y = right_joystick_y;

	return ERR_CODE_SUCCESS;
}
