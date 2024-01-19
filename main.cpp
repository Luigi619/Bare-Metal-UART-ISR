#include "mcal_reg.h"

char msg_ch;

void uart2_write(int ch);
char uart2_read(void);

int main(){
	/*Enable clock to GPIOA*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::ahb1enr, (1U<<0)>::reg_or();
	
	/*Enable clock to UART2*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::apb1enr, (1U<<17)>::reg_or();
	
	/*Configure PA3 as ALT pin UART2 RX*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::gpioa_moder,~(1U<<6)>::reg_and();
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::gpioa_moder,(1U<<7)>::reg_or();
	
	/*Set ALT type to UART2*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::gpioa_afrl,0x7000>::reg_or();
	//mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::gpioa_afrl,(1U<<9)>::reg_or();
	//mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::gpioa_afrl,(1U<<10)>::reg_or();
	//mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::gpioa_afrl,(0U<<11)>::reg_and();
	
	/*Configure USART2 baudrate 9600 @ 16MHz*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_brr,0x0683>::reg_set();
	
	/*Enable Rx, 8-bit*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_cr1,0x04>::reg_set();
	
	/*1 stop bit*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_cr2,0x00>::reg_set();
	
	/*No flow control*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_cr3,0x00>::reg_set();
	
	/*Enabla UART*/
	mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_cr1,0x2000>::reg_or();
	
	while(1){
		msg_ch = uart2_read();
		for(int i = 0; i < 180000; i++){}
	}
}

char uart2_read(void){
	char rcv_ch;
	/*wait until char arrives*/
	while(!(mcal::reg::UART2->SR & 0x0020)){}
		rcv_ch = mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_dr,0U>::reg_get();
		return rcv_ch;
}

void uart2_write(int ch){
	
	
	
	std::uint32_t _ch = ch & 0xFF;
	/*Wait until TX buffer is empty*/
	while(!(mcal::reg::UART2->SR & 0x0080)){}
	//mcal::reg::reg_access<std::uint32_t, std::uint32_t, mcal::reg::uart2_dr,_ch>::reg_set();
		mcal::reg::reg_access_dynamic<std::uint32_t, std::uint32_t>::reg_set(mcal::reg::uart2_dr, _ch);
}