/* This Driver is suitable for SX1276/7/8 Lora module
 * Author: Vinod Kumar from Vinod Embedded
 * Goto: vinodembedded.wordpress.com for detailed explanation of the 
 * lora driver
 */
 
#include "lora.h"
#include <string.h>

uint8_t packetIndex;

uint8_t lora_read_reg(lora_t * module, uint8_t addr) {
	uint8_t txByte = addr & 0x7f;
	uint8_t rxByte = 0x00;
	HAL_GPIO_WritePin(module->pin->nss.port, module->pin->nss.pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->pin->spi, &txByte, 1, 1000);
	while (HAL_SPI_GetState(module->pin->spi) != HAL_SPI_STATE_READY);
	
	HAL_SPI_Receive(module->pin->spi,&rxByte, 1, 1000);
	while(HAL_SPI_GetState(module->pin->spi) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(module->pin->nss.port, module->pin->nss.pin,GPIO_PIN_SET);
	return rxByte;
}

void lora_write_reg(lora_t * module, uint8_t addr, uint8_t cmd){
	uint8_t add = addr | 0x80;
  HAL_GPIO_WritePin(module->pin->nss.port, module->pin->nss.pin,GPIO_PIN_RESET);
  HAL_SPI_Transmit(module->pin->spi, &add, 1, 1000);
	while (HAL_SPI_GetState(module->pin->spi) != HAL_SPI_STATE_READY);
	HAL_SPI_Transmit(module->pin->spi, &cmd, 1, 1000);
	while (HAL_SPI_GetState(module->pin->spi) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(module->pin->nss.port, module->pin->nss.pin,GPIO_PIN_SET);
}



uint8_t lora_init(lora_t * module){
	uint8_t ret;
	HAL_GPIO_WritePin(module->pin->reset.port, module->pin->reset.pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(module->pin->reset.port, module->pin->reset.pin, GPIO_PIN_SET);
  HAL_Delay(10);
	
	ret = lora_read_reg(module, REG_VERSION);
	if(ret != 0x12){
		return 1;
	}
	lora_write_reg(module, REG_OP_MODE, (MODE_LONG_RANGE_MODE | MODE_SLEEP));
	//lora_write_reg(module, REG_FRF_MSB, 0x6C);
	//lora_write_reg(module, REG_FRF_MID, 0x40);
	//lora_write_reg(module, REG_FRF_LSB, 0x00);
	lora_set_frequency(module, FREQUENCY[module->frequency]);
	lora_set_bandwidth(module, BANDWIDTH[module->bandwidth]);
	lora_set_gain(module,module->powergain);
	lora_set_spreading_factor(module, module->spreadingfactor);
	lora_set_coding_rate(module, module->codingrate);
	lora_write_reg(module, REG_FIFO_TX_BASE_ADDR, 0);
	lora_write_reg(module, REG_FIFO_RX_BASE_ADDR, 0);
	ret = lora_read_reg(module, REG_LNA);
	lora_write_reg(module, REG_LNA, ret | 0x03);
	lora_write_reg(module, REG_MODEM_CONFIG_3, 0x04);
	lora_write_reg(module, REG_PA_CONFIG, 0x8f);
	lora_write_reg(module, REG_OP_MODE, (MODE_LONG_RANGE_MODE | MODE_STDBY));
	return 0;
}

int lora_prasePacket(lora_t * module){
	int packetLength = 0, irqFlags; //,ret;
	irqFlags = lora_read_reg(module, REG_IRQ_FLAGS);
	//ret = lora_read_reg(module, REG_MODEM_CONFIG_1);
	lora_write_reg(module, REG_MODEM_CONFIG_1, 0x72);
	
	lora_write_reg(module, REG_IRQ_FLAGS, irqFlags);

	if((irqFlags & IRQ_RX_DONE_MASK) && ((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)) {
		packetLength = lora_read_reg(module, REG_RX_NB_BYTES);
		lora_write_reg(module, REG_FIFO_ADDR_PTR, lora_read_reg(module, REG_FIFO_RX_CURRENT_ADDR));
		lora_write_reg(module, REG_OP_MODE, 0x81);
		packetIndex = 0;
	}
	else if((lora_read_reg(module, REG_OP_MODE)) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE)){
		lora_write_reg(module, REG_FIFO_ADDR_PTR, 0);
		lora_write_reg(module, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
	}
	if((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK)== IRQ_PAYLOAD_CRC_ERROR_MASK){
		return -1;
	}
	return packetLength;
}

uint8_t lora_available(lora_t * module){
	return (lora_read_reg(module, REG_RX_NB_BYTES) - packetIndex);
}

uint8_t lora_read(lora_t * module){
	if(!lora_available(module))
		return 0;
	packetIndex++;
	return lora_read_reg(module, REG_FIFO);
}

uint8_t lora_begin_packet(lora_t * module){
	//int ret;
	if ((lora_read_reg(module, REG_OP_MODE) & MODE_TX) == MODE_TX) {
    return 1;
  }
	lora_write_reg(module, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
	lora_write_reg(module, REG_MODEM_CONFIG_1, 0x72);
	lora_write_reg(module, REG_FIFO_ADDR_PTR, 0);
  lora_write_reg(module, REG_PAYLOAD_LENGTH, 0);
	return 0;
}

void lora_tx(lora_t * module, uint8_t * buf, uint8_t size){
	int currentLength = lora_read_reg(module, REG_PAYLOAD_LENGTH);
  if ((currentLength + size > MAX_PKT_LENGTH)){
    size = MAX_PKT_LENGTH - currentLength;
  }

  for (int i = 0; i < size; i++) {
    lora_write_reg(module, REG_FIFO, buf[i]);
  }
  lora_write_reg(module, REG_PAYLOAD_LENGTH, currentLength + size);
}

uint8_t lora_end_packet(lora_t * module){
	uint8_t timeout = 100;
	lora_write_reg(module, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
  while((lora_read_reg(module,REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0) {
		if(--timeout==0){
			HAL_Delay(1);
			return 1;
		}
  }
  lora_write_reg(module, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
	return 0;
}

void lora_set_frequency(lora_t * module, uint64_t freq){
	uint64_t frf = ((uint64_t)freq << 19) / 32000000;
  lora_write_reg(module, REG_FRF_MSB, (uint8_t)(frf >> 16));
  lora_write_reg(module,REG_FRF_MID, (uint8_t)(frf >> 8));
  lora_write_reg(module,REG_FRF_LSB, (uint8_t)(frf >> 0));
}
void lora_set_bandwidth(lora_t * module, uint8_t bandwidth){
	uint8_t modem_config_1 = lora_read_reg(module, REG_MODEM_CONFIG_1);
	modem_config_1 &= 0x0F;                // Xóa các bit 7-4
	modem_config_1 |= (bandwidth << 4);    // Thiết lập bandwidth
	lora_write_reg(module, REG_MODEM_CONFIG_1, modem_config_1);
}
void lora_set_gain(lora_t * module, uint8_t power_gain){
    // Đọc giá trị hiện tại của thanh ghi REG_PA_CONFIG
    uint8_t pa_config = lora_read_reg(module, REG_PA_CONFIG);
    // Xóa các bit liên quan đến công suất phát (bit 0-3)
    pa_config &= 0xF0;
    // Gán giá trị power_gain trực tiếp vào các bit 0-3
    pa_config |= (power_gain & 0x0F);  // Chỉ lấy phần liên quan đến các bit 0-3
    // Ghi lại giá trị mới vào thanh ghi REG_PA_CONFIG
    lora_write_reg(module, REG_PA_CONFIG, pa_config);
}
void lora_set_spreading_factor(lora_t *module, uint8_t spreading_factor) {
    // Kiểm tra giá trị spreading factor có hợp lệ không
    if (spreading_factor < SF_7) {
        // Nếu SF < 7 thì set = 7
    	spreading_factor =7;
    }
    if (spreading_factor > SF_12) {
        // Nếu SF > 7 thì set = 12
    	spreading_factor =12;
        }
    // Đọc giá trị hiện tại từ thanh ghi REG_MODEM_CONFIG_2
    uint8_t modem_config_2 = lora_read_reg(module, REG_MODEM_CONFIG_2);
    // Xóa các bit 7-4 (liên quan đến SF) để chuẩn bị ghi giá trị mới
    modem_config_2 &= 0x0F;
    // Thiết lập giá trị spreading factor mới
    modem_config_2 |= (spreading_factor << 4);
    // Ghi lại giá trị vào REG_MODEM_CONFIG_2
    lora_write_reg(module, REG_MODEM_CONFIG_2, modem_config_2);
}
void lora_set_coding_rate(lora_t *module, uint8_t coding_rate) {
    // Kiểm tra giá trị coding rate < 1 thì set = 1
    if (coding_rate < CR_4_5) {
        coding_rate =1;
    }
    // Kiểm tra giá trị coding rate > 1 thì set = 4
       if (coding_rate > CR_4_8) {
           coding_rate = 4;
       }
    HAL_Delay(1); // Thêm độ trễ để ổn định trước khi thực hiện bất kỳ tác vụ nào

    // Đọc giá trị hiện tại từ thanh ghi REG_MODEM_CONFIG_1
    uint8_t modem_config_1 = lora_read_reg(module, REG_MODEM_CONFIG_1);
    HAL_Delay(1); // Đợi để đảm bảo đọc hoàn tất

    // Xóa các bit 3-1 (liên quan đến CR) để chuẩn bị ghi giá trị mới
    modem_config_1 &= 0xF1;
    HAL_Delay(1); // Đợi sau khi cập nhật giá trị trong bộ nhớ tạm

    // Thiết lập giá trị coding rate mới (CR nằm ở bit 3-1)
    modem_config_1 |= (coding_rate << 1);
    HAL_Delay(1); // Đợi sau khi chuẩn bị giá trị mới

    // Ghi lại giá trị vào REG_MODEM_CONFIG_1
    lora_write_reg(module, REG_MODEM_CONFIG_1, modem_config_1);
    HAL_Delay(1); // Đợi để đảm bảo quá trình ghi vào thanh ghi hoàn tất
}

void lora_set_mode(lora_t *module, uint8_t mode) {
    // Kiểm tra tính hợp lệ của mode (bit 2-0 của mode)
    if (mode > MODE_RX_SINGLE) {
        return; // Nếu mode không hợp lệ, thoát khỏi hàm
    }

    // Đọc giá trị hiện tại của REG_OP_MODE
    uint8_t op_mode = lora_read_reg(module, REG_OP_MODE);

    // Ghi giá trị mới vào REG_OP_MODE để thiết lập chế độ
    // Xóa các bit 2-0 và ghi mode mới cùng với MODE_LONG_RANGE_MODE
    lora_write_reg(module, REG_OP_MODE, (op_mode & ~0x07) | (MODE_LONG_RANGE_MODE | mode));

    // Đợi một thời gian ngắn để đảm bảo chuyển chế độ thành công
    HAL_Delay(1);
}



/*   End of File
 * Author  : Vinod Kumar
 * Website : vinodembedded.wordpress.com
 */
