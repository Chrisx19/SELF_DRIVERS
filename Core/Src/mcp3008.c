#include "mcp3008.h"

MCP3008_Error_t MCP3008_Init(MCP3008_t *mcp, SPI_HandleTypeDef *hspi1)
{
	MCP3008_Error_t retVal = MCP3008_ERR_SUCCESS;
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	} else {
		mcp->hspi1 = hspi1;
		mcp->mcpConfig = (MCP3008_SINGLE_ENDED | MCP3008_CHANNEL_0);
		mcp->mcpEnable = true;
		mcp->rxCounter = 0;
		mcp->tempBuff[0] = 0;
		mcp->tempBuff[1] = 0;
	}
	printf("MCP3008 Initialized...\r\n");
	return retVal;
}

static MCP3008_Error_t MCP3008_Enable(MCP3008_t *mcp)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	} else {
		mcp->mcpEnable = false;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, mcp->mcpEnable);
	}
	return MCP3008_ERR_SUCCESS;
}

static MCP3008_Error_t MCP3008_Disable(MCP3008_t *mcp)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	} else {
		mcp->mcpEnable = true;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, mcp->mcpEnable);
	}
	return MCP3008_ERR_SUCCESS;
}

static MCP3008_Error_t MCP3008_TransmitRegister(MCP3008_t *mcp, uint8_t const data)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	}

	if (data > 0xFF) {
		return MCP3008_ERR_FAILURE;
	}

	mcp->txBuff = data;
	HAL_SPI_Transmit(mcp->hspi1, &mcp->txBuff, 1, HAL_MAX_DELAY);
//	HAL_SPI_TransmitReceive(hspi, pTxData, pRxData, Size, Timeout)
	return MCP3008_ERR_SUCCESS;
}

static MCP3008_Error_t MCP3008_ReceiveRegister(MCP3008_t *mcp)
{
	if (mcp == NULL) {
		return MCP3008_ERR_FAILURE;
	}

//	if (mcp->rxCounter > 1) {
//		mcp->rxCounter = 0;
//	}

	HAL_SPI_Receive(mcp->hspi1, &mcp->rxBuff, 1, HAL_MAX_DELAY);
	mcp->tempBuff[1] = mcp->rxBuff;
//	mcp->rxCounter++;


	return MCP3008_ERR_SUCCESS;
}

int16_t MCP3008_ReadChannel_1(MCP3008_t *mcp)
{
	int16_t rawVal = 0;
	MCP3008_Error_t retVal = MCP3008_ERR_SUCCESS;
	retVal |= MCP3008_Enable(mcp);
	retVal |= MCP3008_TransmitRegister(mcp, 0x01);
//	retVal |= MCP3008_TransmitRegister(mcp, 0x80);
	mcp->txBuff = 0x80;
	HAL_SPI_TransmitReceive(mcp->hspi1, &mcp->txBuff, &mcp->tempBuff[0], 1, HAL_MAX_DELAY);
	retVal |= MCP3008_ReceiveRegister(mcp);
	retVal |= MCP3008_Disable(mcp);
	if (retVal != MCP3008_ERR_SUCCESS) {
		return 0;
	}

	uint16_t adcMSB = (mcp->tempBuff[0] & 0x03) << 8;
	uint8_t adcLSB = mcp->tempBuff[1];

	rawVal = adcMSB | adcLSB;
    return rawVal;
}
