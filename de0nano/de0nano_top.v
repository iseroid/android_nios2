module de0nano_top(
	input          CLOCK_50,

	output [  7:0] LED,

	input  [  1:0] KEY,

	input  [  3:0] SW,

//	output [ 12:0] DRAM_ADDR,
//	output [  1:0] DRAM_BA,
//	output         DRAM_CAS_N,
//	output         DRAM_CKE,
//	output         DRAM_CLK,
//	output         DRAM_CS_N,
//	inout  [ 15:0] DRAM_DQ,
//	output [  1:0] DRAM_DQM,
//	output         DRAM_RAS_N,
//	output         DRAM_WE_N,

//	output         EPCS_ASDO,
//	input          EPCS_DATA0,
//	output         EPCS_DCLK,
//	output         EPCS_NCSO,

//	output         G_SENSOR_CS_N,
//	input          G_SENSOR_INT,
//	output         I2C_SCLK,
//	inout          I2C_SDAT,

//	output         ADC_CS_N,
//	output         ADC_SADDR,
//	output         ADC_SCLK,
//	input          ADC_SDAT,

//	inout  [ 12:0] GPIO_2,
//	input  [  2:0] GPIO_2_IN,

//	output [ 33:0] GPIO_0_D,
//	input  [  1:0] GPIO_0_IN,

//	output [ 33:0] GPIO_1_D,
//	input  [  1:0] GPIO_1_IN

	// USB Host Shield (GPIO_1)
	output         MAX_RST,
	output         MAX_SS,
	output         MAX_SCLK,
	output         MAX_MOSI,
	input          MAX_MISO,
	input          MAX_INT,
	input          MAX_GPX
);
	wire         clk;
	wire         rst_n;
	wire         w_max_ss;
	wire         w_max_sclk;
	wire         w_max_mosi;
	wire         w_max_miso;
	wire         w_pioin_sw;
	wire [  1:0] w_pioin_usb;
	wire [  7:0] w_pioout_led;

	assign clk = CLOCK_50;
	assign rst_n = KEY[0];
	assign w_max_miso = MAX_MISO;
	assign w_pioin_sw = KEY[1];
	assign w_pioin_usb[0] = MAX_INT;
	assign w_pioin_usb[1] = 1'b1; // MAX_GPX;

	de0nano_sopc de0nano_sopc(
		.MISO_to_the_spi_usb          ( w_max_miso ),
		.MOSI_from_the_spi_usb        ( w_max_mosi ),
		.SCLK_from_the_spi_usb        ( w_max_sclk ),
		.SS_n_from_the_spi_usb        ( w_max_ss ),
		.clk_0                        ( clk ),
		.in_port_to_the_pioin_sw      ( w_pioin_sw ),
		.in_port_to_the_pioin_usb     ( w_pioin_usb ),
		.out_port_from_the_pioout_led ( w_pioout_led ),
		.reset_n                      ( rst_n )
	);


	assign LED = w_pioout_led;
	assign MAX_RST  = rst_n;
	assign MAX_SS   = w_max_ss;
	assign MAX_SCLK = w_max_sclk;
	assign MAX_MOSI = w_max_mosi;

endmodule
