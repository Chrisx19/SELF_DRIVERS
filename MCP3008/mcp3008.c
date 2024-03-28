#include "mcp3008.h"

MCP3008_Error_t MCP3008_Init(MCP3008_t *mcp, SPI_HandleTypeDef *hspi1)
{
	MCP3008_Error_t retVal = MCP3008_ERR_SUCCESS;
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	} else {
		mcp->hspi1 = hspi1;
		memset(mcp->rxBuff, 0x00, RX_SIZE);
	}
	printf("MCP3008 Initialized...\r\n");
	return retVal;
}

static void MCP3008_Enable(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, false);
}

static void MCP3008_Disable(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, true);
}


static MCP3008_Error_t MCP3008_TransmitRegister(MCP3008_t *mcp, const uint8_t data)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	}

	if (data > 0xFF) {
		return MCP3008_ERR_FAILURE;
	}
	HAL_SPI_Transmit(mcp->hspi1, (uint8_t *)&data, 1, HAL_MAX_DELAY);
	return MCP3008_ERR_SUCCESS;
}

static MCP3008_Error_t MCP3008_ReceiveRegister(MCP3008_t *mcp)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	}

	HAL_SPI_Receive_IT(mcp->hspi1, &mcp->rxBuff[1], 1);
	return MCP3008_ERR_SUCCESS;

}

static MCP3008_Error_t MCP3008_TransmitReceiveRegister(MCP3008_t *mcp, uint8_t const data)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	}

	if (data > 0xFF) {
		return MCP3008_ERR_FAILURE;
	}
	HAL_SPI_TransmitReceive(mcp->hspi1, (uint8_t *)&data, &mcp->rxBuff[0], 1, HAL_MAX_DELAY);
	return MCP3008_ERR_SUCCESS;
}



int16_t MCP3008_ReadChannel_1(MCP3008_t *mcp)
{
	const uint8_t START_BIT = 0x01;
	const uint8_t CH1_CONFIG = (MCP3008_SINGLE_ENDED | MCP3008_CHANNEL_0);
	MCP3008_Error_t retVal = MCP3008_ERR_SUCCESS;
	MCP3008_Enable();
	retVal |= MCP3008_TransmitRegister(mcp, START_BIT);
	retVal |= MCP3008_TransmitReceiveRegister(mcp, CH1_CONFIG);
	retVal |= MCP3008_ReceiveRegister(mcp);

	if (retVal != MCP3008_ERR_SUCCESS) {
		return 0;
	}

	uint8_t adcMSB = mcp->rxBuff[0] & 0x03;
	uint8_t adcLSB = mcp->rxBuff[1];

	mcp->adcData = (adcMSB<<8) | adcLSB;
    return mcp->adcData;
}

// This is called when SPI receive is done
void HAL_SPI_RxCpltCallback (SPI_HandleTypeDef *hspi)
{
	MCP3008_Disable();
}
