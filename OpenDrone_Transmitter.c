#include "string.h"
#include "OpenDrone_Transmitter.h"
#include "OpenDrone_Transmitter_HwIntf.h"
#include "OpenDrone_TxProtocol.h"
#include "Periph.h"

#define FREQ_HZ_TO_TIME_US(x)       (1000000.0f/(x))
#define TIME_US_TO_FREQ_HZ(x)       (1000000.0f/(x))

#define IDX_TASK_500_HZ             0
#define IDX_TASK_50_HZ              1
#define IDX_TASK_30_HZ              2
#define IDX_TASK_10_HZ              3
#define IDX_TASK_5_HZ               4
#define NUM_OF_TASK                 5

#define FREQ_500_HZ_TIME_US         FREQ_HZ_TO_TIME_US(500)
#define FREQ_50_HZ_TIME_US          FREQ_HZ_TO_TIME_US(50)
#define FREQ_30_HZ_TIME_US          FREQ_HZ_TO_TIME_US(30)
#define FREQ_10_HZ_TIME_US          FREQ_HZ_TO_TIME_US(10)
#define FREQ_5_HZ_TIME_US           FREQ_HZ_TO_TIME_US(5)

uint32_t last_time_us[NUM_OF_TASK] = {0};
periph_operator_data_t periph_operator_data = {0};
OpenDrone_TxProtocol_Msg_t OpenDrone_TxProtocol_Msg = {0};
OpenDrone_TxProtocol_Msg_OprCtrl_t OpenDrone_TxProtocol_Msg_OprCtrl = {0};
OpenDrone_TxProtocol_Msg_StabilizerCtrl_t OpenDrone_TxProtocol_Msg_StabilizerCtrl = {0};

static err_code_t OpenDrone_Transmitter_InitMessage(void);
static err_code_t OpenDrone_Transmitter_PrepareMessageControl(void);
static err_code_t OpenDrone_Transmitter_SendMessageControl(void);


err_code_t OpenDrone_Transmitter_Init(void)
{
	OpenDrone_Transmitter_InitMessage();
	PeriphSensor_Init();
	PeriphRadio_Init();
	PeriphDisplay_Init();

	return ERR_CODE_SUCCESS;
}

err_code_t OpenDrone_Transmitter_Main(void)
{
	uint32_t current_time = hw_intf_get_time_us();

	/* Task 500 Hz */
	if ((current_time - last_time_us[IDX_TASK_500_HZ]) > FREQ_500_HZ_TIME_US)
	{
		PeriphRadio_ClearTransmitIrqFlags();

		last_time_us[IDX_TASK_500_HZ] = current_time;
	}

	/* Task 50 Hz */
	if ((current_time - last_time_us[IDX_TASK_50_HZ]) > FREQ_50_HZ_TIME_US)
	{
		PeriphSensor_GetJoystickData(&periph_operator_data);

		OpenDrone_Transmitter_PrepareMessageControl();
		OpenDrone_Transmitter_SendMessageControl();

		last_time_us[IDX_TASK_50_HZ] = current_time;
	}

	/* Task 30 Hz */
	if ((current_time - last_time_us[IDX_TASK_30_HZ]) > FREQ_30_HZ_TIME_US)
	{
		last_time_us[IDX_TASK_30_HZ] = current_time;
	}

	/* Task 5 Hz */
	if ((current_time - last_time_us[IDX_TASK_5_HZ]) > FREQ_5_HZ_TIME_US)
	{
		PeriphDisplay_ShowStabilizerMessage(OpenDrone_TxProtocol_Msg_StabilizerCtrl.throttle,
		                                    OpenDrone_TxProtocol_Msg_StabilizerCtrl.roll,
		                                    OpenDrone_TxProtocol_Msg_StabilizerCtrl.pitch,
		                                    OpenDrone_TxProtocol_Msg_StabilizerCtrl.yaw);

		last_time_us[IDX_TASK_5_HZ] = current_time;
	}

	return ERR_CODE_SUCCESS;
}

static err_code_t OpenDrone_Transmitter_InitMessage(void)
{
	OpenDrone_TxProtocol_Msg.StartInd = 0xAA;
	OpenDrone_TxProtocol_Msg.PktLen = 8;
	OpenDrone_TxProtocol_Msg.PktSeq = 0x80;
	OpenDrone_TxProtocol_Msg.SrcId = 0x40;
	OpenDrone_TxProtocol_Msg.DesId = 0x41;
	OpenDrone_TxProtocol_Msg.MsgId = OPENDRONE_TXPROTOCOL_MSG_ID_STABILIZER_CONTROL;
	OpenDrone_TxProtocol_Msg.Crc = 0x00;
	memset(&OpenDrone_TxProtocol_Msg.Payload, 0x00, sizeof(OpenDrone_TxProtocol_Payload_t));

	return ERR_CODE_SUCCESS;
}

static err_code_t OpenDrone_Transmitter_PrepareMessageControl(void)
{
	OpenDrone_TxProtocol_Msg_StabilizerCtrl.throttle = periph_operator_data.left_joystick_y;
	OpenDrone_TxProtocol_Msg_StabilizerCtrl.roll = periph_operator_data.right_joystick_x;
	OpenDrone_TxProtocol_Msg_StabilizerCtrl.pitch = periph_operator_data.right_joystick_y;
	OpenDrone_TxProtocol_Msg_StabilizerCtrl.yaw = periph_operator_data.left_joystick_x;

	OpenDrone_TxProtocol_Msg.Payload = (OpenDrone_TxProtocol_Payload_t)OpenDrone_TxProtocol_Msg_StabilizerCtrl;

	return ERR_CODE_SUCCESS;
}

static err_code_t OpenDrone_Transmitter_SendMessageControl(void)
{
	PeriphRadio_Send((uint8_t *)&OpenDrone_TxProtocol_Msg);

	return ERR_CODE_SUCCESS;
}
