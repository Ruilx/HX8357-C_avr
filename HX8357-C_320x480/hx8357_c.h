/*
 * hx8357_c.h
 *
 * Created: 2017/10/22 周日 0:42:57
 *  Author: Ruilx
 */ 


#ifndef HX8357_C_H_
#define HX8357_C_H_

#include <string.h>
#include "main.h"
#include "utils.h"
#include "ascii.h"
#include "color.h"
// 这些define都没用
#define DataH PORTA
#define DataL PORTC

#define RS_CHANNEL PORTD
#define WR_CHANNEL PORTG
#define CS_CHANNEL PORTG
#define RST_CHANNEL PORTG

class HX8357C{
	uint8 rs = 0;
	uint8 rw = 0;
	uint8 cs = 0;
	uint8 rst = 0;

	uint16 LcdWidth = 320;
	uint16 LcdHeight = 480;
	uint8 colorBits = 16;
public:
	enum Orientation{
		Portrait = 0x00,
		Landspace = 0x01,
	};
private:
	Orientation ori = Portrait;
	Color backColor;
	Color foreColor;
public:

	enum Command{
		NOP = 0x00, SWRESET = 0x01, RDDIDIF = 0x04, RDNUMPE = 0x05, RDRED = 0x06, RDGREEN = 0x07, RDBLUE = 0x08, RDDPM = 0x0A, RDDMADCTL = 0x0B, RDDCOLMOD = 0x0C, RDDIM = 0x0D, RDDSM = 0x0E, RDDSDR = 0x0F,
		SLPIN = 0x10, SLPOUT = 0x11, PTLON = 0x12, NORON = 0x13,
		INVOFF = 0x20, INVON = 0x21, ALLPOFF = 0x22, ALLPON = 0x23, GAMSET = 0x26, DISPOFF = 0x28, DISPON = 0x29, CASET = 0x2A, PASET = 0x2B, RAMWR = 0x2C, RAMRD = 0x2E,
		PLTAR = 0x30, VSCRDEF = 0x33, TEOFF = 0x34, TEON = 0x35, MADCTL = 0x36, VSCRSADD = 0x37, IDMOFF = 0x38, IDMON = 0x39, COLMOD = 0x3A, RAMWRCON = 0x3C, RAMRDCON = 0x3E,
		TESL = 0x44, GETSCAN = 0x45,
		WRDISBV = 0x51, RDDISBV = 0x52, WRCTRLD = 0x53, RDCTRLD = 0x54, WRCABC = 0x55, RDCABC = 0x56, WRCABCMB = 0x5E, RDCABCMB = 0x5F,
		RDABCSDR = 0x68,
		RDBWLB = 0x70, RDBkx = 0x71, RDBky = 0x72, RDWx = 0x73, RDWy = 0x74, RDRGLB = 0x75, RDRx = 0x76, RDRy = 0x77, RDGx = 0x78, RDGy = 0x79, RDBALB = 0x7A, RDBx = 0x7B, RDBy = 0x7C, RDAx = 0x7D, RDAy = 0x7E,
		Read_DDB_start = 0xA1, Read_DDB_continue = 0xA8, RDFCS = 0xAA, RDCCS = 0xAF, RDID1 = 0xDA, RDID2 = 0xDB, RDID3 = 0xDC,

		SETOSC = 0xB0, SETPOWER = 0xB1, SETDISPLAY = 0xB2, SETRGB = 0xB3, SETCYC = 0xB4, SETBGP = 0xB5, SETVCOM = 0xB6, SETOTP = 0xB7, SETEXTC = 0xB9,
		SETDGC = 0xC1, SETSTBA = 0xC0, SETID = 0xC3, SETDDB = 0xC4, SETCABC = 0xC9, SETPANEL = 0xCC,
		SETGAMMA = 0xE0, SETIMAGEI = 0xE9, SETMESSI = 0xEA, SETCOLOR = 0xEB,
		SETREADINDEX = 0xFE, GETSPIREAD = 0xFF,
	};

public:
	inline void begin(){
		Utils::WG(this->cs, false);
	}

	inline void end(){
		Utils::WG(this->cs, true);
	}

	inline uint16 getLcdWidth() const{
		return this->ori == Portrait ? this->LcdWidth : this->LcdHeight;
	}

	inline uint16 getLcdHeight() const{
		return this->ori == Portrait ? this->LcdHeight : this->LcdWidth;
	}

	inline uint8 getLcdColorDepth() const{
		return this->colorBits;
	}

	inline Color getForeColor() const{
		return this->foreColor;
	}

	inline Color getBackColor() const{
		return this->backColor;
	}
protected:
	inline void sendCommand(Command command){
		Utils::WD(this->rs, false);
		Utils::WA(0x00);
		Utils::WC(command);
		Utils::WG(this->rw, false);
		Utils::WG(this->rw, true);
	}
	
	inline void sendData(uint8 data){
		Utils::WD(this->rs, true);
		Utils::WA(0x00);
		Utils::WC(data);
		Utils::WG(this->rw, false);
		Utils::WG(this->rw, true);
	}
	
	inline void sendData16(uint16 data){
		Utils::WD(this->rs, true);
		Utils::WA(data >> 8);
		Utils::WC(data & 0xFF);
		Utils::WG(this->rw, false);
		Utils::WG(this->rw, true);
	}

	inline void sendSameData(){
		Utils::WG(this->rw, false);
		Utils::WG(this->rw, true);
	}

	inline void sendData16(uint16 data, uint16 times){
		Utils::WD(this->rs, true);
		Utils::WA(data >> 8);
		Utils::WC(data & 0xFF);
		while(times--){
			Utils::WG(this->rw, false);
			Utils::WG(this->rw, true);
		}
	}

public:
	HX8357C(){

	}

	HX8357C(uint8 rs, uint8 rw, uint8 cs, uint8 rst){
		this->rs = rs;
		this->rw = rw;
		this->cs = cs;
		this->rst = rst;

		Utils::DA(Utils::Output);
		Utils::DC(Utils::Output);
		Utils::DD(this->rs, Utils::Output);
		Utils::DG(this->rw, Utils::Output);
		Utils::DG(this->cs, Utils::Output);
		Utils::DG(this->rst, Utils::Output);

		Utils::WA(0x00);
		Utils::WC(0x00);
		Utils::WD(this->rs, false);
		Utils::WG(this->rw, false);
		Utils::WG(this->cs, true);
		Utils::WG(this->rst, false);
		_delay_ms(50);
		Utils::WG(this->rst, true);
		_delay_ms(100);

		this->backColor = Color(0, 0, 0);
		this->foreColor = Color(255, 255, 255);

		this->initLcd();
	}

	void nop(){
		this->sendCommand(NOP);
	}

	void softwareReset(){
		this->sendCommand(SWRESET);
	}

	void sleepIn(){
		this->sendCommand(SLPIN);
	}

	void sleepOut(){
		this->sendCommand(SLPOUT);
	}

	void partialModeOn(){
		this->sendCommand(PTLON);
	}

	void normalDisplayModeOn(){
		this->sendCommand(NORON);
	}

	void displayInversionOff(){
		this->sendCommand(INVOFF);
	}

	void displayInversionOn(){
		this->sendCommand(INVON);
	}

	void allPixelOff(){
		this->sendCommand(ALLPOFF);
	}

	void allPixelOn(){
		this->sendCommand(ALLPON);
	}

	void gammaSet(uint8 curve){
		this->sendCommand(GAMSET);
		this->sendData(curve);
	}

	void displayOff(){
		this->sendCommand(DISPOFF);
	}

	void displayOn(){
		this->sendCommand(DISPON);
	}

	void columnAddressSet(uint16 startCol, uint16 endCol){
		this->sendCommand(CASET);
		this->sendData(startCol >> 8);
		this->sendData(startCol & 0xFF);
		this->sendData(endCol >> 8);
		this->sendData(endCol & 0xFF);
	}

	void pageAddressSet(uint16 startPage, uint16 endPage){
		this->sendCommand(PASET);
		this->sendData(startPage >> 8);
		this->sendData(startPage & 0xFF);
		this->sendData(endPage >> 8);
		this->sendData(endPage & 0xFF);
	}

	void memoryWrite(uint16 *data, uint16 length){
		this->sendCommand(RAMWR);
		uint16 *startData = data;
		uint16 *endData = data + length;
		while(startData < endData){
			this->sendData16(*startData);
			startData++;
		}
	}

	void memoryWrite(uint16 unit, uint16 length){
		this->sendCommand(RAMWR);
		this->sendData16(unit, length);
		//length--;
		//while(length--){
		//	this->sendSameData();
		//}
	}

	void memoryWrite(uint16 unit_p1, uint16 unit_p2, uint16 unit_p3, uint16 length){
		this->sendCommand(RAMWR);
		length <<= 1;
		while(length--){
			this->sendData16(unit_p1);
			this->sendData16(unit_p2);
			this->sendData16(unit_p3);
		}
	}

	void memoryWrite(uint16 unit, uint16 lengthTimes, uint16 lengthPattarn){
		this->sendCommand(RAMWR);
		while(lengthTimes--){
			this->sendData16(unit, lengthPattarn);
		}
	}

	void memoryWrite(uint16 unit_p1, uint16 unit_p2, uint16 unit_p3, uint16 lengthTimes, uint16 lengthPattern){
		this->sendCommand(RAMWR);
		lengthTimes <<= 1;
		while(lengthTimes--){
			uint16 p = lengthPattern;
			while(p--){
				this->sendData16(unit_p1);
				this->sendData16(unit_p2);
				this->sendData16(unit_p3);
			}
		}
	}

	void keepWrite(uint16 *data, uint16 length){
		uint16 *startData = data;
		uint16 *endData = data + length;
		while(startData < endData){
			this->sendData16(*startData);
			startData++;
		}
	}

	void partialArea(uint16 startRow, uint16 endRow){
		this->sendCommand(PLTAR);
		this->sendData(startRow >> 8);
		this->sendData(startRow & 0xFF);
		this->sendData(endRow >> 8);
		this->sendData(endRow & 0xFF);
	}

	void verticalScrollingDefinition(uint16 topFixedArea, uint16 viewScrollArea, uint16 bottomFixedArea){
		this->sendCommand(VSCRDEF);
		this->sendData(topFixedArea >> 8);
		this->sendData(topFixedArea & 0xFF);
		this->sendData(viewScrollArea >> 8);
		this->sendData(viewScrollArea & 0xFF);
		this->sendData(bottomFixedArea >> 8);
		this->sendData(bottomFixedArea & 0xFF);
	}

	void tearingEffectLineOff(){
		this->sendCommand(TEOFF);
	}

	void tearingEffectLineOn(bool temode){
		this->sendCommand(TEON);
		this->sendData(temode? 0x01: 0x00);
	}

	enum MemoryAccessWRDirection{
		PageAddressOrder = 0x80,
		ColumnAddressOrder = 0x40,
		PageColumnSelection = 0x20,
	};
	void memoryAccessControl(uchar dir, bool verticalOrder, bool rgbBgrOrder, bool horizontalOrder){
		if(dir & 0x20){
			this->ori = Landspace;
			//this->LcdHeight = 320;
			//this->LcdWidth = 480;
		}else{
			this->ori = Portrait;
			//this->LcdHeight = 480;
			//this->LcdWidth = 320;
		}
		this->sendCommand(MADCTL);
		this->sendData(dir | (rgbBgrOrder? 0x08: 0x00) | (horizontalOrder? 0x04: 0x00) | (verticalOrder? 0x10: 0x00));
	}
	void memoryAccessControl(uchar dat){
		this->sendCommand(MADCTL);
		this->sendData(dat);
	}
	/*
	void memoryAccessControl(uint8 dir, bool verticalOrder, bool rgbBgrOrder, bool horizontalOrder){
		if(dir & PageColumnSelection == PageColumnSelection){
			this->ori = Landspace;
		}else{
			this->ori = Portrait;
		}
		this->sendCommand(MADCTL);
		this->sendData(dir | (rgbBgrOrder? 0x08: 0x00) | (horizontalOrder? 0x04: 0x00) | (verticalOrder? 0x10: 0x00));
	}
	*/
	/*
	void memoryAccessControl(MemoryAccessWRDirection dir, bool verticalOrder, bool rgbBgrOrder, bool horizontalOrder, bool VerticalOrder){
		this->sendCommand(MADCTL);
		this->sendData(dir | (verticalOrder? 0x10: 0x00) | (rgbBgrOrder? 0x08: 0x00) | (horizontalOrder? 0x02: 0x00) | (verticalOrder? 0x01: 0x00));
	}
	*/
	void verticalScrollingStartAddress(uint16 verticalScrollingPage){
		this->sendCommand(VSCRSADD);
		this->sendData(verticalScrollingPage >> 8);
		this->sendData(verticalScrollingPage & 0xFF);
	}

	void idleModeOff(){
		this->sendCommand(IDMOFF);
	}

	void idleModeOn(){
		this->sendCommand(IDMON);
	}

	enum DbiInterfaceFormat{
		Dbi_NotDefined = 0x00,
		Dbi_12BitPixel = 0x03,
		Dbi_16BitPixel = 0x05,
		Dbi_18BitPixel = 0x06,
		Dbi_24BitPixel = 0x07,
	};

	enum DpiInterfaceFormat{
		Dpi_NotDefined = 0x00,
		Dpi_16BitPixel = 0x05,
		Dpi_18BitPixel = 0x06,
		Dpi_24BitPixel = 0x07,
	};

	void interfacePixelFormat(DbiInterfaceFormat dbi, DpiInterfaceFormat dpi){
		this->colorBits = dbi == Dbi_12BitPixel? 12: (dbi == Dbi_16BitPixel? 16: (dbi == Dbi_18BitPixel? 18: (dbi == Dbi_24BitPixel? 24: 16)));
		this->sendCommand(COLMOD);
		this->sendData(dpi << 4 | dbi);
	}

	void writeMemoryContinue(uint8 *data, uint16 length){
		this->sendCommand(RAMWRCON);
		uint8 *startData = data;
		uint8 *endData = data + length;
		while(startData < endData){
			this->sendData(*startData);
			startData++;
		}
	}

	void setTearScanLine(uint16 teline){
		this->sendCommand(TESL);
		this->sendData(teline >> 8);
		this->sendData(teline & 0xFF);
	}

	void writeDisplayBrightnessValue(uint8 value){
		this->sendCommand(WRDISBV);
		this->sendData(value);
	}

	void writeControlDisplay(bool backlightControlBlockOnOff, bool displayDimming, bool backlightControlOnOff){
		this->sendCommand(WRCTRLD);
		this->sendData((backlightControlBlockOnOff? 0x20: 0x00) | (displayDimming? 0x08: 0x00) | (backlightControlOnOff? 0x04: 0x00));
	}

	enum WriteContentAdaptiveBrightnessControlMode{
		WCABC_Off = 0,
		WCABC_UserInterfaceImage = 1,
		WCABC_StillPicture = 2,
		WCABC_MovingImage = 3,
	};

	void writeContentAdaptiveBrightnessControl(WriteContentAdaptiveBrightnessControlMode mode){
		this->sendCommand(WRCABC);
		this->sendData(mode);
	}

	void writeCabcMinimumBrightness(uint8 value){
		this->sendCommand(WRCABCMB);
		this->sendData(value);
	}

	enum OscillatorRelatedSetting{
		IOF_50Percent_10MHz = 0,
		IOF_58Percent_10MHz = 1,
		IOF_67Percent_10MHz = 2,
		IOF_75Percent_10MHz = 3,
		IOF_83Percent_10MHz = 4,
		IOF_92Percent_10MHz = 5,
		IOF_100Percent_10MHz = 6,
		IOF_108Percent_10MHz = 7,
		IOF_116Percent_10MHz = 8,
		IOF_125Percent_10MHz = 9,
		IOF_133Percent_10MHz = 10,
		IOF_142Percent_10MHz = 11,
		IOF_150Percent_10MHz = 12,
		IOF_158Percent_10MHz = 13,
		IOF_167Percent_10MHz = 14,
		IOF_175Percent_10MHz = 15,
	};

	void setInternalOscillator(OscillatorRelatedSetting i_uadj, OscillatorRelatedSetting n_uadj, bool osc_en){
		this->sendCommand(SETOSC);
		this->sendData(i_uadj << 4 | n_uadj);
		this->sendData(osc_en? 0x01: 0x00);
	}

	enum PowerOutputFactor_BT{
		SPBT_VSP5V_VGH3VSP_VGLN3VSP = 0,
		SPBT_VSP5V_VGH3VSP_VGLN2VSP = 1,
		SPBT_VSP5V_VGH3VSP_VGLN2VSP_X = 2,
		SPBT_VSP5V_VGHVCIA2VSP_VGLNVCIN2VSP = 3,
		SPBT_VSP5V_VGHVCIA2VSP_VGLN2VSP = 4,
		SPBT_VSP5V_VGHVCIA2VSP_VGLVCIN2VSP = 5,
		SPBT_VSP5V_VGH2VSP_VGLN2VSP = 6,
		SPBT_VSP5V_VGH2VSP_VGLVCINVSP = 7,
	};

	enum PowerControlVoltageAdjusting{
		// TRI = 0 Or TRI = 1
		SPVRH_3V09 = 0x00,
		SPVRH_3V14 = 0x01,
		SPVRH_3V19 = 0x02,
		SPVRH_3V23 = 0x03,
		SPVRH_3V28 = 0x04,
		SPVRH_3V33 = 0x05,
		SPVRH_3V38 = 0x06,
		SPVRH_3V42 = 0x07,
		SPVRH_3V47 = 0x08,
		SPVRH_3V52 = 0x09,
		SPVRH_3V56 = 0x0A,
		SPVRH_3V61 = 0x0B,
		SPVRH_3V66 = 0x0C,
		SPVRH_3V70 = 0x0D,
		SPVRH_3V75 = 0x0E,
		SPVRH_3V80 = 0x0F,
		SPVRH_3V84 = 0x10,
		SPVRH_3V89 = 0x11,
		SPVRH_3V94 = 0x12,
		SPVRH_3V98 = 0x13,
		SPVRH_4V03 = 0x14,
		SPVRH_4V08 = 0x15,
		SPVRH_4V13 = 0x16,
		SPVRH_4V17 = 0x17,
		SPVRH_4V22 = 0x18,
		SPVRH_4V27 = 0x19,
		SPVRH_4V31 = 0x1A,
		SPVRH_4V36 = 0x1B,
		SPVRH_4V41 = 0x1C,
		SPVRH_4V45 = 0x1D,
		SPVRH_4V50 = 0x1E,
		// TRI = 1
		SPVRH_4V55 = 0x1F,
		SPVRH_4V59 = 0x20,
		SPVRH_4V64 = 0x21,
		SPVRH_4V69 = 0x22,
		SPVRH_4V73 = 0x23,
		SPVRH_4V78 = 0x24,
		SPVRH_4V83 = 0x25,
		SPVRH_4V88 = 0x26,
		SPVRH_4V92 = 0x27,
		SPVRH_4V97 = 0x28,
		SPVRH_5V02 = 0x29,
		SPVRH_5V06 = 0x2A,
		SPVRH_5V11 = 0x2B,
		SPVRH_5V16 = 0x2C,
		SPVRH_5V20 = 0x2D,
		SPVRH_5V25 = 0x2E,
		SPVRH_5V30 = 0x2F,
		SPVRH_5V34 = 0x30,
		SPVRH_5V39 = 0x31,
		SPVRH_5V44 = 0x32,
	};

	enum PowerControlConstantCurrentOfOperationalAmplifer{
		SPAP_OperationOfTheOperationalAmpliferStops = 0,
		SPAP_Small_0 = 1,
		SPAP_Small_1 = 2,
		SPAP_Small_2 = 3,
		SPAP_Medium = 4,
		SPAP_MediumHigh = 5,
		SPAP_Large = 6,
		SPAP_Small_3 = 7,
	};

	enum PowerControlOperatingFrequencyForVspAndVsn{
		SPOFVV_SettingInhibited = 0,
		SPOFVV_FoscDiv32 = 1,
		SPOFVV_FoscDiv48 = 2,
		SPOFVV_FoscDiv64 = 3,
		SPOFVV_FoscDiv80 = 4,
		SPOFVV_FoscDiv96 = 5,
		SPOFVV_FoscDiv112 = 6,
		SPOFVV_FoscDiv128 = 7,
		SPOFVV_FoscDiv288 = 8,
		SPOFVV_FoscDiv320 = 9,
		SPOFVV_FoscDiv352 = 10,
		SPOFVV_FoscDiv384 = 11,
		SPOFVV_FoscDiv416 = 12,
		SPOFVV_FoscDiv448 = 13,
		SPOFVV_FoscDiv480 = 14,
		SPOFVV_FoscDiv512 = 15,
	};

	void setPowerControl(bool dp_stb, bool tri, bool xdk,
						 PowerOutputFactor_BT bt,
						 PowerControlVoltageAdjusting vrh,
						 PowerControlVoltageAdjusting nvrh,
						 bool gasen,
						 PowerControlConstantCurrentOfOperationalAmplifer ap,
						 PowerControlOperatingFrequencyForVspAndVsn fs1,
						 PowerControlOperatingFrequencyForVspAndVsn fs0,
						 bool vcomg,
						 bool pon,
						 bool dk,
						 bool stb){
		this->sendCommand(SETPOWER);
		this->sendData(dp_stb? 0x01: 0x00);
		this->sendData((tri? 0x20: 0x00) | (xdk? 0x10: 0x00) | bt);
		this->sendData(vrh);
		this->sendData(nvrh);
		this->sendData((gasen? 0x80: 0x00) | ap);
		this->sendData(fs1 << 4 | fs0);
		this->sendData((vcomg? 0x40: 0x00) | (pon? 0x10: 0x00) | (dk? 0x08: 0x00) | (stb? 0x01: 0x00));
	}

	enum ScanCycle{
		ISC_1Frame = 0x00,
		ISC_5Frame = 0x01,
		ISC_9Frame = 0x02,
		ISC_13Frame = 0x03,
		ISC_17Frame = 0x04,
		ISC_21Frame = 0x05,
		ISC_25Frame = 0x06,
		ISC_29Frame = 0x07,
		ISC_33Frame = 0x08,
		ISC_37Frame = 0x09,
		ISC_41Frame = 0x0A,
		ISC_45Frame = 0x0B,
		ISC_49Frame = 0x0C,
		ISC_53Frame = 0x0D,
		ISC_57Frame = 0x0E,
		ISC_SettingInhibited = 0x0F,
	};

	enum SourceOutputLevel{
		PT_NDA_V0N_V63P = 0,
		PT_NDA_V0N_V63P_1 = 1,
		PT_NDA_GND = 2,
		PT_NDA_HiZ = 3,
	};

	enum GateOutput{
		GONDTE_FixedToVGH = 0,
		GONDTE_FixedToVGH_2 = 1,
		GONDTE_FixedToVGL = 2,
		GONDTE_NormalOperation = 3
	};

	enum SourceOutput{
		D_VSSD_Halt_Halt = 0,
		D_VSSD_Operate_Operate = 1,
		D_PT00_Operate_Operate = 2,
		D_Display_Operate_Operate = 3,
	};

	void setDisplayRelatedRegister(ScanCycle isc,
								   SourceOutputLevel pt,
								   bool ptg, bool ref,
								   uint8 nl,
								   uint8 scn,
								   GateOutput gon_dte,
								   SourceOutput d){
		this->sendCommand(SETDISPLAY);
		this->sendData(isc);
		this->sendData(pt << 6 | (ptg? 0x02: 0x00) | (ref? 0x01: 0x00));
		this->sendData(nl);
		this->sendData(scn);
		this->sendData(gon_dte << 4 | d << 2);
	}

	enum RgbInterface16BitsColorMapping{
		RI_CopyRedBlueHigherBitToLowerBit = 0,
		RI_Using0ForLowerBit = 1,
		RI_Using1ForLowerBit = 2,
		RI_CopyGreenLowerBitToRedBlueLowerBit = 3,
	};

	void setRgbInterface(bool sdo_en, bool bypass, RgbInterface16BitsColorMapping epf,
						 bool rm, bool dm,
						 bool epl, bool vspl, bool hspl, bool dpl,
						 bool rcm, uint8 hbp, uint8 vbp){
		this->sendCommand(SETRGB);
		this->sendData((sdo_en? 0x80: 0x00) | (bypass? 0x40: 0x00) | (epf << 4) | (rm? 0x02: 0x00) | (dm? 0x01: 0x00));
		this->sendData((dpl? 0x08: 0x00) | (hspl? 0x04: 0x00) | (vspl? 0x02: 0x00) | (epl? 0x01: 0x00));
		this->sendData((rcm? 0x80: 0x00) | hbp);
		this->sendData(vbp);
	}

	enum LcdDrivingInversionType{
		ColumnInversion = 0x0,
		_1DotInversion = 0x1,
		_2DotInversion = 0x2,
		_4DorInversion = 0x3,
	};

	enum InternalDisplayOperationClockFrequency{
		FoscDiv1 = 0,
		FoscDiv2 = 1,
		FoscDiv4 = 2,
		FoscDiv8 = 3,
	};

	void setDisplayCycleRegister(bool zinv, LcdDrivingInversionType i_nw, LcdDrivingInversionType n_nw,
								 uint8 rtn,
								 bool osc_div2, InternalDisplayOperationClockFrequency div,
								 uint8 n_dum,
								 uint8 i_dum,
								 uint8 gdon,
								 uint8 gdof){
		this->sendCommand(SETCYC);
		this->sendData((zinv? 0x80: 0x00) | (i_nw << 4) | n_nw);
		this->sendData(rtn & 0x7F);
		this->sendData((osc_div2? 0x08: 0x00) | div);
		this->sendData(n_dum);
		this->sendData(i_dum);
		this->sendData(gdon);
		this->sendData(gdof);
	}

	enum Vref{
		Vref_4V1_5V1 = 0,
		Vref_4V2_5V2 = 1,
		Vref_4V3_5V3 = 2,
		Vref_4V4_5V4 = 3,
		Vref_4V45_5V45 = 4,
		Vref_4V5_5V5 = 5,
		Vref_4V55_5V55 = 6,
		Vref_4V6_5V6 = 7,
		Vref_4V65_5V65 = 8,
		Vref_4V7_5V7 = 9,
		Vref_4V75_5V75 = 0xA,
		Vref_4V8_5V8 = 0xB,
		Vref_SettingDisable = 0xC,
	};

	enum Vvdhs{
		Vvdhs_4v64_6v08 = 0,
		Vvdhs_4v80_6v37 = 1,
		Vvdhs_5v07_6v68 = 2,
		Vvdhs_5v26_5v59 = 3,
		Vvdhs_5v35_5v70 = 4,
		Vvdhs_5v47_5v82 = 5,
		Vvdhs_5v59_5v95 = 6,
		Vvdhs_5v70_8v05 = 7,
	};

	enum Vpp{
		Vpp_7V2 = 0,
		Vpp_7V3 = 1,
		Vpp_7V4 = 2,
		Vpp_7V5 = 3,
		Vpp_7V6 = 4,
		Vpp_7V7 = 5,
		Vpp_7V8 = 6,
		Vpp_External = 7,

	};

	void setPowerOption(Vref vref,
						Vref nVref,
						Vpp vpp, Vvdhs vvdhs){
		this->sendCommand(SETBGP);
		this->sendData(vref & 0x0F);
		this->sendData(nVref & 0x0F);
		this->sendData(vpp << 5 | vvdhs);
	}

	enum VcomOtpTimes{
		VOTP_NotProgrammed = 0,
		VOTP_1Time = 1,
		VOTP_2Times = 2,
		VOTP_3Times = 3,
		VOTP_4Times = 4,
	};

	void setVcomVoltage(uint8 vcom,
						VcomOtpTimes /* vcom_otp_times */){
		this->sendCommand(SETVCOM);
		this->sendData(vcom & 0x7F);
		//this->sendData(vcom_otp_times);
	}

	void setOtpSetting(uint8 otp_key, uint8 otp_mask, uint8 otp_index,
					   bool load_ids, bool vpp_en, bool otp_por, bool vpp_sel, bool otp_prog,
					   uint8 otp_dout){
		this->sendCommand(SETOTP);
		this->sendData(otp_key);
		this->sendData(otp_mask);
		this->sendData(otp_index);
		this->sendData((load_ids? 0x80: 0x00) | (vpp_en? 0x40: 0x00) | (otp_por? 0x20: 0x00) | (vpp_sel? 0x20: 0x00) | (otp_prog? 0x01: 0x00));
		this->sendData(otp_dout);
	}

	void enableExtensionCommand(bool enable){
		this->sendCommand(SETEXTC);
		this->sendData(0xFF);
		this->sendData(0x83);
		this->sendData(0x57 & (enable? 0x00: 0xFF));
	}

	void setSourceOption(uint8 np_opon, uint8 iPi_open, uint8 stbaHi8, uint8 stbaMid8, uint8 stbaLo8, uint8 genon){
		this->sendCommand(SETSTBA);
		this->sendData(np_opon);
		this->sendData(iPi_open);
		this->sendData(stbaHi8);
		this->sendData(stbaMid8);
		this->sendData(stbaLo8);
		this->sendData(genon);
	}

	void setDgcRelatedSetting(bool gdc_en, uint8 gdc_lut_r[33], uint8 gdc_lut_g[33], uint8 gdc_lut_b[33]){
		this->sendCommand(SETDGC);
		this->sendData(gdc_en? 0x01: 0x00);
		for(uint8 i = 0; i < 33; i++){
			this->sendData(gdc_lut_r[i]);
		}
		for(uint8 i = 0; i < 33; i++){
			this->sendData(gdc_lut_g[i]);
		}
		for(uint8 i = 0; i < 33; i++){
			this->sendData(gdc_lut_b[i]);
		}
	}

	void setDdb(uint8 ddb1, uint8 ddb2, uint8 ddb3, uint8 ddb4){
		this->sendCommand(SETDDB);
		this->sendData(ddb1);
		this->sendData(ddb2);
		this->sendData(ddb3);
		this->sendData(ddb4);
	}

	enum BrightnessControlClock{
		BCC_PWM_CLK_Div1 = 0,
		BCC_PWM_CLK_Div2 = 1,
		BCC_PWM_CLK_Div4 = 2,
		BCC_PWM_CLK_Div8 = 3,
		BCC_PWM_CLK_Div16 = 4,
		BCC_PWM_CLK_Div32 = 5,
		BCC_PWM_CLK_Div64 = 6,
		BCC_PWM_CLK_Div128 = 7,
	};

	void setCabcRelatedSetting(bool bc_on, BrightnessControlClock pwmdiv, bool invplus,
							   uint8 pwm_period){
		this->sendCommand(SETCABC);
		this->sendData((bc_on? 0x80: 0x00) | (pwmdiv << 4) | (invplus? 0x02: 0x00) | 0x0D);
		this->sendData(pwm_period);
	}

	void setPanelCharacteristicRegister(bool sm_panel, bool ss_panel, bool gs_panel, bool rev_panel, bool bgr_panel){
		this->sendCommand(SETPANEL);
		this->sendData((sm_panel? 0x10: 0x00) | (ss_panel? 0x08: 0x00) | (gs_panel? 0x04: 0x00) | (rev_panel? 0x02: 0x00) | (bgr_panel? 0x01: 0x00));
	}

	void setGamma(uint8 vrp[16], uint8 vrn[16], uint8 cgn, bool gma_reload){
		this->sendCommand(SETGAMMA);
		for(uint8 i = 0; i < 16; i++){
			this->sendData(vrp[i]);
		}
		for(uint8 i = 0; i < 16; i++){
			this->sendData(vrn[i]);
		}
		this->sendData(cgn);
		this->sendData(gma_reload? 0x01: 0x00);
	}

	void setImageFunction(bool pwm2_en, bool dith_en, bool yuv_en, bool db_en){
		this->sendCommand(SETIMAGEI);
		this->sendData((pwm2_en? 0x80: 0x00) | (dith_en? 0x20: 0x00) | (yuv_en? 0x10: 0x00) | (db_en? 0x01: 0x00));
	}

	void setCommandType(bool spi_dcx_sel, bool messi_enb){
		this->sendCommand(SETMESSI);
		this->sendData((spi_dcx_sel? 0x02: 0x00) | (messi_enb? 0x01: 0x00));
	}

	void setColor(uint8 colorpara[15]){
		this->sendCommand(SETCOLOR);
		for(int i = 0; i < 15; i++){
			this->sendData(colorpara[i]);
		}
	}

	void setSpiReadIndex(uint8 cmd_add){
		this->sendCommand(SETREADINDEX);
		this->sendData(cmd_add);
	}

	void sendOperation(Command cmd, uint8 *data, uint8 length){
		uint8 *dataPtr = data;
		this->sendCommand(cmd);
		for(int i = 0; i < length; i++){
			this->sendData(*dataPtr);
			dataPtr++;
		}
	}
protected:
	void initLcd(){
		this->begin();
		this->softwareReset();
		this->displayOff();
		this->enableExtensionCommand(true);
		//this->setVcomVoltage(0x2C, VOTP_NotProgrammed); // Set VCOM Voltage: -1.52V

		this->sleepOut();
		this->tearingEffectLineOn(false);
		_delay_ms(100);
		this->interfacePixelFormat(Dbi_16BitPixel, Dpi_NotDefined);
		this->setPowerControl(false, false, true, SPBT_VSP5V_VGHVCIA2VSP_VGLVCIN2VSP, SPVRH_3V33, SPVRH_3V33, true, SPAP_Small_2, SPOFVV_FoscDiv80, SPOFVV_FoscDiv288, false, false, false, false);
		//this->setSourceOption(0x24, 0x24, 0x01, 0x3C, 0xC8, 0x08);
		this->setDisplayCycleRegister(false, ColumnInversion, _2DotInversion, 0x40, false, FoscDiv1, 0x2A, 0x2A, 0x0D, 0x4F);
		uint8 vrp[16] = {0x00, 0x15, 0x1D, 0x2A, 0x31, 0x42, 0x4C, 0x53, 0x45, 0x40, 0x3B, 0x32, 0x2E, 0x28, 0x24, 0x03};
		uint8 vrn[16] = {0x00, 0x15, 0x1D, 0x2A, 0x31, 0x42, 0x4C, 0x53, 0x45, 0x40, 0x3B, 0x32, 0x2E, 0x28, 0x24, 0x03};
		this->setGamma(vrp, vrn, 0x00, true);
		//this->setRgbInterface(false, true, RI_CopyRedBlueHigherBitToLowerBit, true, true, false, false, false, false, false, 0x06, 0x06);
		//this->memoryAccessControl(ColumnAddressOrder, true, true, false);
		this->memoryAccessControl(PageAddressOrder | PageColumnSelection | ColumnAddressOrder, false, true, false);
		//this->memoryAccessControl(0xE8);
		this->clearScr(0, 0, 0);
		this->displayOn();

/*
		this->setForeColor(250, 250, 250);
		this->setBackColor(0, 0, 0);
		this->draw6x8Str("Booting System... Screen set complete!", 10, 158);
		this->draw6x8Str("Hello, zhiyb! Welcome to MegaOS 1.0.", 10, 166);
*/
		
		this->end();
	}

public:
	inline void setRect(uint16 x, uint16 y, uint16 width, uint16 height){
		this->columnAddressSet(x, x + width -1);
		this->pageAddressSet(y, y + height -1);
	}

	inline void writeColor(uint16 *color, uint16 length){
		this->memoryWrite(color, length);
	}

	inline void fillColor(uint16 color, uint16 width, uint16 height){
		this->memoryWrite(color, width, height);
	}

	inline void fillColor(uchar red, uchar green, uchar blue, uint16 width, uint16 height){
		switch(this->colorBits){
			case 24:
				this->memoryWrite(((uint16)red << 8) | green, ((uint16)blue << 8) | red, ((uint16)green << 8) | blue, height, width);
				break;
			case 16:
				this->memoryWrite(ConC24To16((unsigned long)red << 16 | green << 8 | blue), height, width);
				break;
			default:
				break;
		}
	}

	inline void fillColor(Color color, uint16 width, uint16 height){
		switch(this->colorBits){
			case 24:
				this->memoryWrite(((uint16)color.getRed() << 8) | color.getGreen(), ((uint16)color.getBlue() << 8) | color.getRed(), ((uint16)color.getGreen() << 8) | color.getBlue(), height, width);
				break;
			case 16:
				this->memoryWrite(color.getRgb565(), height, width);
				break;
			default:
				break;
		}
	}

	inline void clearScr(uint8 red, uint8 green, uint8 blue){
		if(this->ori == Portrait){
			this->setRect(0, 0, 480, 320);
			this->fillColor(red, green, blue, 320, 480);
		}else{
			this->setRect(0, 0, 480, 320);
			this->fillColor(red, green, blue, 480, 320);
		}
	}

	inline void clearScr(uint16 color){
		if(this->ori == Portrait){
			this->setRect(0, 0, 320, 480);
			this->fillColor(ConC16To24_R(color), ConC16To24_G(color), ConC16To24_B(color), 320, 480);
		}else{
			this->setRect(0, 0, 480, 320);
			this->fillColor(ConC16To24_R(color), ConC16To24_G(color), ConC16To24_B(color), 480, 320);
		}
	}

	void setBackColor(uint8 red, uint8 green, uint8 blue){
		this->backColor.setRed(red);
		this->backColor.setGreen(green);
		this->backColor.setBlue(blue);
	}

	void setForeColor(uint8 red, uint8 green, uint8 blue){
		this->foreColor.setRed(red);
		this->foreColor.setGreen(green);
		this->foreColor.setBlue(blue);
	}

	void drawBin(uchar *buf, uint16 x, uint16 y, uint16 w, uint16 h){
		uint16 chLine[8];
		int16 length = w * h;
		uint8 *pbuf = buf;
		//uint8 i = 0;
		this->setRect(x, y, w, h);
		uint16 fC = this->foreColor.getRgb565();
		uint16 bC = this->backColor.getRgb565();
		this->sendCommand(RAMWR);
		while(length > 0){
			uint8 cch = *pbuf;
			for(uint8 j = 0; j < 8; j++){
				chLine[j] = (cch & 0x80) ? fC : bC;
				cch <<= 1;
			}
			pbuf++;
			this->keepWrite(chLine, 8);
			length -= 8;
		}
	}

	void draw6x8Char(uchar ch, uint16 x, uint16 y){
		uint8 chTable[8] = {0};
		uint16 fC = this->foreColor.getRgb565();
		uint16 bC = this->backColor.getRgb565();
		getAscii(ch, chTable);
		this->setRect(x, y, 6, 8);
		uint16 chLine[48] = {0};
		for(uint8 i = 0; i < 8; i++){
			chLine[i * 6] = (chTable[i] >> 7)? fC: bC;
			chTable[i] &= 0x7F;
			chLine[(i * 6) + 1] = (chTable[i] >> 6)? fC: bC;
			chTable[i] &= 0x3F;
			chLine[(i * 6) + 2] = (chTable[i] >> 5)? fC: bC;
			chTable[i] &= 0x1F;
			chLine[(i * 6) + 3] = (chTable[i] >> 4)? fC: bC;
			chTable[i] &= 0x0F;
			chLine[(i * 6) + 4] = (chTable[i] >> 3)? fC: bC;
			chTable[i] &= 0x07;
			chLine[(i * 6) + 5] = (chTable[i] >> 2)? fC: bC;
		}
		this->memoryWrite(chLine, 48);
	}

	void draw6x8Str(char *str, uint16 x, uint16 y){
		/*uint8 maxLen = strlen(str) > (this->LcdWidth / 6)? this->LcdWidth / 6: strlen(str);
		this->setRect(x, y, maxLen * 6, 8);
		for(uint8 i = 0; i < 8; i++){
			uint8 chTable[80];
			for(uint8 j = 0; j < maxLen; j++){
				uint8 line = getAsciiLine(str[j], i);
				chTable[j]
			}
		}*/
		uint8 maxLen = strlen(str); // > (this->getLcdWidth() / 6)? this->getLcdWidth() / 6: strlen(str);
		while(maxLen--){
			this->draw6x8Char(*str, x, y);
			str++;
			x += 6;
		}
	}

	void drawPoint(uint16 x, uint16 y){
		this->setRect(x, y, 1, 1);
		this->memoryWrite(this->foreColor.getRgb565(), 1);
	}

	void drawHLine(uint16 x, uint16 y, uint16 length){
		this->setRect(x, y, length, 1);
		this->memoryWrite(this->foreColor.getRgb565(), length);
	}

	void drawVLine(uint16 x, uint16 y, uint16 length){
		this->setRect(x, y, 1, length);
		this->memoryWrite(this->foreColor.getRgb565(), length);
	}
};

#endif /* HX8357_C_H_ */
