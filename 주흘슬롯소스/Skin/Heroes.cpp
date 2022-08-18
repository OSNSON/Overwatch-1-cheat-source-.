#include "Heroes.h"

using namespace std;
extern Heroes myHero;


LPCSTR Hero2Str(Heroes h)
{
	switch (h)
	{
	case ANA: return "ANA";
	case ASHE: return "ASHE";
	case BARTIST: return "BARTIST";
	case BASTION: return "BASTION";
	case BRIGITTE: return "BRIGITTE";
	case DOOMFIST: return "DOOMFIST";
	case DVA: return "D.VA";
	case GENJI: return "GENJI";
	case HANZO: return "HANZO";
	case JUNKRAT: return "JUNKRAT";
	case LUCIO: return "LUCIO";
	case MCCREE: return "MCCREE";
	case MEI: return "MEI";
	case MERCY: return "MERCY";
	case MOIRA: return "MOIRA";
	case ORISA: return "ORISA";
	case PHARAH: return "PHARAH";
	case REAPER: return "REAPER";
	case REINHARDT: return "REINHARDT";
	case ROADHOG: return "ROADHOG";
	case SOLDIER: return "SOLDIER";
	case SOMBRA: return "SOMBRA";
	case SYMMETRA: return "SYMMETRA";
	case TORBJORN: return "TORBJORN";
	case TRACER: return "TRACER";
	case WIDOWMAKER: return "WIDOWMAKER";
	case WINSTON: return "WINSTON";
	case WRECKINGBALL: return "WRECKINGBALL";
	case ZARYA: return "ZARYA";
	case ZENYATTA: return "ZENYATTA";
	case SIGMA: return "SIGMA";
	case ECHO: return "ECHO";
	case TRAINING_BOT: return "TRAINING_BOT";
	case AllHero: return "AllHero";
	default: return "XXX";
	}
}

Heroes Skin2Hero(WORD Skin)
{
	Heroes Hero = XXX;

	if (Skin == 0x1921
		|| Skin == 0x1923
		|| Skin == 0x1925
		|| Skin == 0x1924
		|| Skin == 0x1922
		|| Skin == 0x1927
		|| Skin == 0x1926
		|| Skin == 0x199C
		|| Skin == 0x1B11
		|| Skin == 0x194B
		|| Skin == 0x194A
		|| Skin == 0x1950
		|| Skin == 0x1951
		|| Skin == 0x1BB8
		|| Skin == 0x1B0F
		|| Skin == 0x1C0F // 여기 까지 기본스킨
		|| Skin == 0x1DEF
		|| Skin == 0x1F06
		|| Skin == 0x1DF2
		|| Skin == 0x1F07
		|| Skin == 0x2AF5
		|| Skin == 0x2AD8
		|| Skin == 0x1DFB
		|| Skin == 0x1F08
		|| Skin == 0x1DE9
		|| Skin == 0x1F02
		|| Skin == 0x1DEC
		|| Skin == 0x1F05
		|| Skin == 0x2BC0
		|| Skin == 0x2C51
		|| Skin == 0x1DE6
		|| Skin == 0x1EFE
		|| Skin == 0x1E04
		|| Skin == 0x1F0C
		|| Skin == 0x1DFE
		|| Skin == 0x1F0A
		|| Skin == 0x1E01
		|| Skin == 0x1F0B
		|| Skin == 0x2C34
		|| Skin == 0x2C17
		|| Skin == 0x2BFA
		|| Skin == 0x2BDD
		|| Skin == 0x2ABB
		|| Skin == 0x2AA0
		|| Skin == 0x2BA3
		|| Skin == 0x2B86
		|| Skin == 0x2B69
		|| Skin == 0x2B4C
		|| Skin == 0x1DF5
		|| Skin == 0x1F03
		|| Skin == 0x1CAC
		|| Skin == 0x1F09
		|| Skin == 0x2B2F
		|| Skin == 0x2B12
		|| Skin == 0x1DF8
		|| Skin == 0x1F04)
	{
		Hero = static_cast<Heroes>(0);
	}

	if (Skin == 0x1ED3
		|| Skin == 0x2A03
		|| Skin == 0x2A01
		|| Skin == 0x2A02
		|| Skin == 0x2A04
		|| Skin == 0x2A05
		|| Skin == 0x2A06
		|| Skin == 0x2A10
		|| Skin == 0x2A07
		|| Skin == 0x21CF
		|| Skin == 0x2A09
		|| Skin == 0x21D2
		|| Skin == 0x2A13 // 여기까지 기본스킨
		|| Skin == 0x2A29
		|| Skin == 0x2A36
		|| Skin == 0x2A2A
		|| Skin == 0x2A37
		|| Skin == 0x2B0D
		|| Skin == 0x2AF0
		|| Skin == 0x2A2D
		|| Skin == 0x2A38
		|| Skin == 0x2A27
		|| Skin == 0x2A32
		|| Skin == 0x2A28
		|| Skin == 0x2A35
		|| Skin == 0x2BD8
		|| Skin == 0x2C69
		|| Skin == 0x2A18
		|| Skin == 0x2A19
		|| Skin == 0x2A31
		|| Skin == 0x2A3C
		|| Skin == 0x2A2F
		|| Skin == 0x2A3A
		|| Skin == 0x2A30
		|| Skin == 0x2A3B
		|| Skin == 0x2C4C
		|| Skin == 0x2C2F
		|| Skin == 0x2C12
		|| Skin == 0x2BF5
		|| Skin == 0x2AD3
		|| Skin == 0x2AB8
		|| Skin == 0x2BBB
		|| Skin == 0x2B9E
		|| Skin == 0x2B81
		|| Skin == 0x2B64
		|| Skin == 0x2A2B
		|| Skin == 0x2A33
		|| Skin == 0x2A2E
		|| Skin == 0x2A39
		|| Skin == 0x2B47
		|| Skin == 0x2B2A
		|| Skin == 0x2A2C
		|| Skin == 0x2A34)
	{
		Hero = static_cast<Heroes>(1);
	}

	if (Skin == 0x2188
		|| Skin == 0x2C87
		|| Skin == 0x2C8A
		|| Skin == 0x2C89
		|| Skin == 0x2C88
		|| Skin == 0x2C92
		|| Skin == 0x2C90
		|| Skin == 0x2CDD
		|| Skin == 0x2C77
		|| Skin == 0x2CDB
		|| Skin == 0x2C7F
		|| Skin == 0x2C7A // 여기까지 기본스킨
		|| Skin == 0x2CB1
		|| Skin == 0x2CBF
		|| Skin == 0x2CB2
		|| Skin == 0x2CC0
		|| Skin == 0x2CCA
		|| Skin == 0x2CCB
		|| Skin == 0x2CB5
		|| Skin == 0x2CB9
		|| Skin == 0x2CB0
		|| Skin == 0x2CBB
		|| Skin == 0x2CAF
		|| Skin == 0x2CBE
		|| Skin == 0x2CC3
		|| Skin == 0x2CCF
		|| Skin == 0x2CA7
		|| Skin == 0x2CBA
		|| Skin == 0x2CB8
		|| Skin == 0x2CAA
		|| Skin == 0x2CB7
		|| Skin == 0x2CAC
		|| Skin == 0x2CAE
		|| Skin == 0x2CAB
		|| Skin == 0x2CA9
		|| Skin == 0x2CA8
		|| Skin == 0x2CC1
		|| Skin == 0x2CC2
		|| Skin == 0x2CCC
		|| Skin == 0x2CCD
		|| Skin == 0x2CC4
		|| Skin == 0x2CC5
		|| Skin == 0x2CC6
		|| Skin == 0x2CC7
		|| Skin == 0x2CB3
		|| Skin == 0x2CBC
		|| Skin == 0x2CB6
		|| Skin == 0x2CAD
		|| Skin == 0x2CC8
		|| Skin == 0x2CC9
		|| Skin == 0x2CB4
		|| Skin == 0x2CBD)
	{
		Hero = static_cast<Heroes>(2);
	}

	if (Skin == 0x1627
		|| Skin == 0x1B90
		|| Skin == 0x1E97
		|| Skin == 0x18CC
		|| Skin == 0x162A
		|| Skin == 0x1629
		|| Skin == 0x1628
		|| Skin == 0x18CE
		|| Skin == 0x18CD
		|| Skin == 0x199F
		|| Skin == 0x2A46
		|| Skin == 0x1AE8
		|| Skin == 0x1B93
		|| Skin == 0x1798
		|| Skin == 0x1799
		|| Skin == 0x1794
		|| Skin == 0x1795
		|| Skin == 0x1C3E
		|| Skin == 0x2A58
		|| Skin == 0x1623 // 여기까지 기본스킨
		|| Skin == 0x1D4C
		|| Skin == 0x202D
		|| Skin == 0x1D60
		|| Skin == 0x202E
		|| Skin == 0x2B03
		|| Skin == 0x2AE6
		|| Skin == 0x1D9C
		|| Skin == 0x202F
		|| Skin == 0x1D24
		|| Skin == 0x2029
		|| Skin == 0x1D38
		|| Skin == 0x202C
		|| Skin == 0x2BCE
		|| Skin == 0x2C5F
		|| Skin == 0x1D10
		|| Skin == 0x2028
		|| Skin == 0x1DD8
		|| Skin == 0x2033
		|| Skin == 0x1DB0
		|| Skin == 0x2031
		|| Skin == 0x1DC4
		|| Skin == 0x2032
		|| Skin == 0x2C42
		|| Skin == 0x2C25
		|| Skin == 0x2C08
		|| Skin == 0x2BEB
		|| Skin == 0x2AC9
		|| Skin == 0x2AAE
		|| Skin == 0x2BB1
		|| Skin == 0x2B94
		|| Skin == 0x2B77
		|| Skin == 0x2B5A
		|| Skin == 0x1D74
		|| Skin == 0x202A
		|| Skin == 0x1C9E
		|| Skin == 0x2030
		|| Skin == 0x2B3D
		|| Skin == 0x2B20
		|| Skin == 0x1D88
		|| Skin == 0x202B)
	{
		Hero = static_cast<Heroes>(3);
	}

	if (Skin == 0x1BCE
		|| Skin == 0x1EA8
		|| Skin == 0x1EA5
		|| Skin == 0x1EAA
		|| Skin == 0x1EA9
		|| Skin == 0x1EAD
		|| Skin == 0x1EAB
		|| Skin == 0x2131
		|| Skin == 0x2A54
		|| Skin == 0x1EC2
		|| Skin == 0x1E80
		|| Skin == 0x1E81
		|| Skin == 0x1EC3
		|| Skin == 0x1EBC
		|| Skin == 0x2A69 // 여기까지 기본스킨
		|| Skin == 0x1EC8
		|| Skin == 0x20A3
		|| Skin == 0x1EC9
		|| Skin == 0x20A4
		|| Skin == 0x2AF3
		|| Skin == 0x2AD6
		|| Skin == 0x1ECC
		|| Skin == 0x20A5
		|| Skin == 0x1EC6
		|| Skin == 0x209F
		|| Skin == 0x1EC7
		|| Skin == 0x20A2
		|| Skin == 0x2BBE
		|| Skin == 0x2C4F
		|| Skin == 0x1EC5
		|| Skin == 0x209E
		|| Skin == 0x1ED0
		|| Skin == 0x20A9
		|| Skin == 0x1ECE
		|| Skin == 0x20A7
		|| Skin == 0x1ECF
		|| Skin == 0x20A8
		|| Skin == 0x2C32
		|| Skin == 0x2C15
		|| Skin == 0x2BF8
		|| Skin == 0x2BDB
		|| Skin == 0x2AB9
		|| Skin == 0x2A9E
		|| Skin == 0x2BA1
		|| Skin == 0x2B84
		|| Skin == 0x2B67
		|| Skin == 0x2B4A
		|| Skin == 0x1ECA
		|| Skin == 0x20A0
		|| Skin == 0x1ECD
		|| Skin == 0x20A6
		|| Skin == 0x2B2D
		|| Skin == 0x2B10
		|| Skin == 0x1ECB
		|| Skin == 0x20A1)
	{
		Hero = static_cast<Heroes>(4);
	}

	if (Skin == 0x160F
		|| Skin == 0x1611
		|| Skin == 0x1BDA
		|| Skin == 0x1C71
		|| Skin == 0x18C3
		|| Skin == 0x1612
		|| Skin == 0x1610
		|| Skin == 0x18C4
		|| Skin == 0x18C5
		|| Skin == 0x2CD2
		|| Skin == 0x1C14
		|| Skin == 0x1BE3
		|| Skin == 0x1BD9
		|| Skin == 0x1BE3
		|| Skin == 0x1BE4
		|| Skin == 0x2104
		|| Skin == 0x1C15 // 여기까지 기본스킨
		|| Skin == 0x1D57
		|| Skin == 0x2011
		|| Skin == 0x1D6B
		|| Skin == 0x2012
		|| Skin == 0x2AF6
		|| Skin == 0x2AD9
		|| Skin == 0x1DA7
		|| Skin == 0x2013
		|| Skin == 0x1D2F
		|| Skin == 0x200D
		|| Skin == 0x1D43
		|| Skin == 0x2010
		|| Skin == 0x2BC1
		|| Skin == 0x2C52
		|| Skin == 0x1D1B
		|| Skin == 0x200C
		|| Skin == 0x1DE3
		|| Skin == 0x2017
		|| Skin == 0x1DBB
		|| Skin == 0x2015
		|| Skin == 0x1DCF
		|| Skin == 0x2016
		|| Skin == 0x2C35
		|| Skin == 0x2C18
		|| Skin == 0x2BFB
		|| Skin == 0x2BDE
		|| Skin == 0x2ABC
		|| Skin == 0x2AA1
		|| Skin == 0x2BA4
		|| Skin == 0x2B87
		|| Skin == 0x2B6A
		|| Skin == 0x2B4D
		|| Skin == 0x1D7F
		|| Skin == 0x200E
		|| Skin == 0x1CAB
		|| Skin == 0x2014
		|| Skin == 0x2B30
		|| Skin == 0x2B13
		|| Skin == 0x1D93
		|| Skin == 0x200F
		)
	{
		Hero = static_cast<Heroes>(5);
	}

	if (Skin == 0x1613
		|| Skin == 0x1614
		|| Skin == 0x1616
		|| Skin == 0x1615
		|| Skin == 0x18C6
		|| Skin == 0x18C7
		|| Skin == 0x18C8
		|| Skin == 0x197A
		|| Skin == 0x2CCE
		|| Skin == 0x17A1
		|| Skin == 0x17A0
		|| Skin == 0x1764
		|| Skin == 0x1765
		|| Skin == 0x1C3D
		|| Skin == 0x1BB0
		|| Skin == 0x1EA2
		|| Skin == 0x1A19
		|| Skin == 0x196C
		|| Skin == 0x2A57 // 여기까지 기본스킨
		|| Skin == 0x1D54
		|| Skin == 0x2061
		|| Skin == 0x1D68
		|| Skin == 0x2062
		|| Skin == 0x2AF9
		|| Skin == 0x2ADC
		|| Skin == 0x1DA4
		|| Skin == 0x2063
		|| Skin == 0x1D2C
		|| Skin == 0x205D
		|| Skin == 0x1D40
		|| Skin == 0x2060
		|| Skin == 0x2BC4
		|| Skin == 0x2C55
		|| Skin == 0x1D18
		|| Skin == 0x205C
		|| Skin == 0x1DE0
		|| Skin == 0x2067
		|| Skin == 0x1DB8
		|| Skin == 0x2065
		|| Skin == 0x1DCC
		|| Skin == 0x2066
		|| Skin == 0x2C38
		|| Skin == 0x2C1B
		|| Skin == 0x2BFE
		|| Skin == 0x2BE1
		|| Skin == 0x2ABF
		|| Skin == 0x2AA4
		|| Skin == 0x2BA7
		|| Skin == 0x2B8A
		|| Skin == 0x2B6D
		|| Skin == 0x2B50
		|| Skin == 0x1D7C
		|| Skin == 0x205E
		|| Skin == 0x1CA8
		|| Skin == 0x2064
		|| Skin == 0x2B33
		|| Skin == 0x2B16
		|| Skin == 0x1D90
		|| Skin == 0x205F
		)
	{
		Hero = static_cast<Heroes>(6);
	}


	if (Skin == 0x16D7
		|| Skin == 0x18EA
		|| Skin == 0x18EB
		|| Skin == 0x16D9
		|| Skin == 0x16D8
		|| Skin == 0x16DA
		|| Skin == 0x18EC
		|| Skin == 0x197C
		|| Skin == 0x1785
		|| Skin == 0x1784
		|| Skin == 0x1788
		|| Skin == 0x1789
		|| Skin == 0x1994
		|| Skin == 0x1EC1
		|| Skin == 0x1C1D
		|| Skin == 0x476A
		|| Skin == 0x4789
		|| Skin == 0x1B7D
		|| Skin == 0x1B91 // 여기까지 기본스킨
		|| Skin == 0x1D01
		|| Skin == 0x20B8
		|| Skin == 0x1D02
		|| Skin == 0x20B9
		|| Skin == 0x2B00
		|| Skin == 0x2AE3
		|| Skin == 0x1D05
		|| Skin == 0x20BA
		|| Skin == 0x1CFF
		|| Skin == 0x20B4
		|| Skin == 0x1D00
		|| Skin == 0x20B7
		|| Skin == 0x2BCB
		|| Skin == 0x2C5C
		|| Skin == 0x1CFE
		|| Skin == 0x20B3
		|| Skin == 0x1D08
		|| Skin == 0x20BE
		|| Skin == 0x1D06
		|| Skin == 0x20BC
		|| Skin == 0x1D07
		|| Skin == 0x20BD
		|| Skin == 0x2C3F
		|| Skin == 0x2C22
		|| Skin == 0x2C05
		|| Skin == 0x2BE8
		|| Skin == 0x2AC6
		|| Skin == 0x2AAB
		|| Skin == 0x2BAE
		|| Skin == 0x2B91
		|| Skin == 0x2B74
		|| Skin == 0x2B57
		|| Skin == 0x1D03
		|| Skin == 0x20B5
		|| Skin == 0x1CA1
		|| Skin == 0x20BB
		|| Skin == 0x2B3A
		|| Skin == 0x2B1D
		|| Skin == 0x1D04
		|| Skin == 0x20B6
		|| Skin == 0x1994
		)
	{
		Hero = static_cast<Heroes>(7); //겐지 genji GENJI
	}

	if (Skin == 0x15EB
		|| Skin == 0x15ED
		|| Skin == 0x18B4
		|| Skin == 0x15EC
		|| Skin == 0x15EE
		|| Skin == 0x18B5
		|| Skin == 0x18B6
		|| Skin == 0x19A7
		|| Skin == 0x19B4
		|| Skin == 0x15E4
		|| Skin == 0x15E3
		|| Skin == 0x15E8
		|| Skin == 0x15E7
		|| Skin == 0x2103
		|| Skin == 0x19B2
		|| Skin == 0x2129
		|| Skin == 0x1C28
		|| Skin == 0x19B0 // 여기까지 기본스킨
		|| Skin == 0x1DEE
		|| Skin == 0x1F6A
		|| Skin == 0x1DF1
		|| Skin == 0x1F6B
		|| Skin == 0x2B09
		|| Skin == 0x2AEC
		|| Skin == 0x1DFA
		|| Skin == 0x1F6C
		|| Skin == 0x1DE8
		|| Skin == 0x1F66
		|| Skin == 0x1DEB
		|| Skin == 0x1F69
		|| Skin == 0x2BD4
		|| Skin == 0x2C65
		|| Skin == 0x1DE5
		|| Skin == 0x1F65
		|| Skin == 0x1E03
		|| Skin == 0x1F70
		|| Skin == 0x1DFD
		|| Skin == 0x1F6E
		|| Skin == 0x1E00
		|| Skin == 0x1F6F
		|| Skin == 0x2C48
		|| Skin == 0x2C2B
		|| Skin == 0x2C0E
		|| Skin == 0x2BF1
		|| Skin == 0x2ACF
		|| Skin == 0x2AB4
		|| Skin == 0x2BB7
		|| Skin == 0x2B9A
		|| Skin == 0x2B7D
		|| Skin == 0x2B60
		|| Skin == 0x1DF4
		|| Skin == 0x1F67
		|| Skin == 0x1C98
		|| Skin == 0x1F6D
		|| Skin == 0x2B43
		|| Skin == 0x2B26
		|| Skin == 0x1DF7
		|| Skin == 0x1F68
		)
	{
		Hero = static_cast<Heroes>(8);
	}

	if (Skin == 0x1607
		|| Skin == 0x1608
		|| Skin == 0x177C
		|| Skin == 0x18BD
		|| Skin == 0x160A
		|| Skin == 0x1609
		|| Skin == 0x18BF
		|| Skin == 0x18BE
		|| Skin == 0x1C4D
		|| Skin == 0x1A2D
		|| Skin == 0x2CE5
		|| Skin == 0x179D
		|| Skin == 0x179C
		|| Skin == 0x177D
		|| Skin == 0x1BC1
		|| Skin == 0x1962
		|| Skin == 0x1C13
		|| Skin == 0x20E7
		|| Skin == 0x1C73 // 여기까지 기본스킨
		|| Skin == 0x1CF6
		|| Skin == 0x1FEB
		|| Skin == 0x1CF7
		|| Skin == 0x1FEC
		|| Skin == 0x2AFD
		|| Skin == 0x2AE0
		|| Skin == 0x1CFA
		|| Skin == 0x1FED
		|| Skin == 0x1CF4
		|| Skin == 0x1FE7
		|| Skin == 0x1CF5
		|| Skin == 0x1FEA
		|| Skin == 0x2BC8
		|| Skin == 0x2C59
		|| Skin == 0x1CF3
		|| Skin == 0x1FE6
		|| Skin == 0x1CFD
		|| Skin == 0x1FF1
		|| Skin == 0x1CFB
		|| Skin == 0x1FEF
		|| Skin == 0x1CFC
		|| Skin == 0x1FF0
		|| Skin == 0x2C3C
		|| Skin == 0x2C1F
		|| Skin == 0x2C02
		|| Skin == 0x2BE5
		|| Skin == 0x2AC3
		|| Skin == 0x2AA8
		|| Skin == 0x2BAB
		|| Skin == 0x2B8E
		|| Skin == 0x2B71
		|| Skin == 0x2B54
		|| Skin == 0x1CF8
		|| Skin == 0x1FE8
		|| Skin == 0x1CA4
		|| Skin == 0x1FEE
		|| Skin == 0x2B37
		|| Skin == 0x2B1A
		|| Skin == 0x1CF9
		|| Skin == 0x1FE9
		)
	{
		Hero = static_cast<Heroes>(9);
	}

	if (Skin == 0x160B
		|| Skin == 0x18C2
		|| Skin == 0x160D
		|| Skin == 0x160E
		|| Skin == 0x18C0
		|| Skin == 0x160C
		|| Skin == 0x18C1
		|| Skin == 0x1A0B
		|| Skin == 0x20EC
		|| Skin == 0x1BC2
		|| Skin == 0x1778
		|| Skin == 0x1779
		|| Skin == 0x1910
		|| Skin == 0x1911
		|| Skin == 0x1947
		|| Skin == 0x1946
		|| Skin == 0x257D
		|| Skin == 0x1E2F
		|| Skin == 0x1B94 // 여기까지 기본스킨
		|| Skin == 0x1D53
		|| Skin == 0x1FFA
		|| Skin == 0x1D67
		|| Skin == 0x1FFB
		|| Skin == 0x2AFA
		|| Skin == 0x2ADD
		|| Skin == 0x1DA3
		|| Skin == 0x1FFC
		|| Skin == 0x1D2B
		|| Skin == 0x1FF6
		|| Skin == 0x1D3F
		|| Skin == 0x1FF9
		|| Skin == 0x2BC5
		|| Skin == 0x2C56
		|| Skin == 0x1D17
		|| Skin == 0x1FF5
		|| Skin == 0x1DDF
		|| Skin == 0x2000
		|| Skin == 0x1DB7
		|| Skin == 0x1FFE
		|| Skin == 0x1DCB
		|| Skin == 0x1FFF
		|| Skin == 0x2C39
		|| Skin == 0x2C1C
		|| Skin == 0x2BFF
		|| Skin == 0x2BE2
		|| Skin == 0x2AC0
		|| Skin == 0x2AA5
		|| Skin == 0x2BA8
		|| Skin == 0x2B8B
		|| Skin == 0x2B6E
		|| Skin == 0x2B51
		|| Skin == 0x1D7B
		|| Skin == 0x1FF7
		|| Skin == 0x1CA7
		|| Skin == 0x1FFD
		|| Skin == 0x2B34
		|| Skin == 0x2B17
		|| Skin == 0x1D8F
		|| Skin == 0x1FF8
		)
	{
		Hero = static_cast<Heroes>(10);
	}

	if (Skin == 0x15F7
		|| Skin == 0x15F9
		|| Skin == 0x1C4C
		|| Skin == 0x15FA
		|| Skin == 0x15F8
		|| Skin == 0x18B7
		|| Skin == 0x18B8
		|| Skin == 0x18B9
		|| Skin == 0x1976
		|| Skin == 0x21CD
		|| Skin == 0x1A12
		|| Skin == 0x15F3
		|| Skin == 0x15F4
		|| Skin == 0x15EF
		|| Skin == 0x15F0
		|| Skin == 0x1BCD
		|| Skin == 0x1BCA
		|| Skin == 0x1C12
		|| Skin == 0x2A08
		|| Skin == 0x19B6
		|| Skin == 0x1E0C // 여기까지 기본스킨
		|| Skin == 0x1D50
		|| Skin == 0x1FBF
		|| Skin == 0x1D64
		|| Skin == 0x1FC0
		|| Skin == 0x2AFF
		|| Skin == 0x2AE2
		|| Skin == 0x1DA0
		|| Skin == 0x1FC1
		|| Skin == 0x1D28
		|| Skin == 0x1FBB
		|| Skin == 0x1D3C
		|| Skin == 0x1FBE
		|| Skin == 0x2BCA
		|| Skin == 0x2C5B
		|| Skin == 0x1D14
		|| Skin == 0x1FBA
		|| Skin == 0x1DDC
		|| Skin == 0x1FB9
		|| Skin == 0x1DB4
		|| Skin == 0x1FC3
		|| Skin == 0x1DC8
		|| Skin == 0x1FC4
		|| Skin == 0x2C3E
		|| Skin == 0x2C21
		|| Skin == 0x2C04
		|| Skin == 0x2BE7
		|| Skin == 0x2AC5
		|| Skin == 0x2AAA
		|| Skin == 0x2BAD
		|| Skin == 0x2B90
		|| Skin == 0x2B73
		|| Skin == 0x2B56
		|| Skin == 0x1D78
		|| Skin == 0x1FBC
		|| Skin == 0x1CA3
		|| Skin == 0x1FC2
		|| Skin == 0x2B39
		|| Skin == 0x2B1C
		|| Skin == 0x1D8C
		|| Skin == 0x1FBD
		)
	{
		Hero = static_cast<Heroes>(11);
	}

	if (Skin == 0x161F
		|| Skin == 0x1621
		|| Skin == 0x1620
		|| Skin == 0x18C9
		|| Skin == 0x1622
		|| Skin == 0x18CA
		|| Skin == 0x18CB
		|| Skin == 0x21AC
		|| Skin == 0x1BE1
		|| Skin == 0x17A4
		|| Skin == 0x1914
		|| Skin == 0x1BEC
		|| Skin == 0x1A10
		|| Skin == 0x1982
		|| Skin == 0x1981
		|| Skin == 0x1C72
		|| Skin == 0x1B67
		|| Skin == 0x29F9 // 여기까지 기본스킨
		|| Skin == 0x1D55
		|| Skin == 0x201E
		|| Skin == 0x1D5B
		|| Skin == 0x201F
		|| Skin == 0x2AF8
		|| Skin == 0x2ADB
		|| Skin == 0x1DA5
		|| Skin == 0x2020
		|| Skin == 0x1D2D
		|| Skin == 0x201A
		|| Skin == 0x1D41
		|| Skin == 0x201D
		|| Skin == 0x2BC3
		|| Skin == 0x2C54
		|| Skin == 0x1D19
		|| Skin == 0x2019
		|| Skin == 0x1DE1
		|| Skin == 0x2024
		|| Skin == 0x1DB9
		|| Skin == 0x2022
		|| Skin == 0x1DCD
		|| Skin == 0x2023
		|| Skin == 0x2C37
		|| Skin == 0x2C1A
		|| Skin == 0x2BFD
		|| Skin == 0x2BE0
		|| Skin == 0x2ABE
		|| Skin == 0x2AA3
		|| Skin == 0x2BA6
		|| Skin == 0x2B89
		|| Skin == 0x2B6C
		|| Skin == 0x2B4F
		|| Skin == 0x1D7D
		|| Skin == 0x201B
		|| Skin == 0x1CA9
		|| Skin == 0x2021
		|| Skin == 0x2B32
		|| Skin == 0x2B15
		|| Skin == 0x1D91
		|| Skin == 0x201C
		)
	{
		Hero = static_cast<Heroes>(12);
	}

	if (Skin == 0x15DB
		|| Skin == 0x15DE
		|| Skin == 0x18AE
		|| Skin == 0x15DD
		|| Skin == 0x15DC
		|| Skin == 0x18AF
		|| Skin == 0x18B0
		|| Skin == 0x1979
		|| Skin == 0x1A2A
		|| Skin == 0x15D3
		|| Skin == 0x15D4
		|| Skin == 0x15D7
		|| Skin == 0x15D8
		|| Skin == 0x1BC6
		|| Skin == 0x196A
		|| Skin == 0x1F71
		|| Skin == 0x1C11
		|| Skin == 0x1B7B // 여기까지 기본스킨
		|| Skin == 0x1D47
		|| Skin == 0x1F5D
		|| Skin == 0x1D5B
		|| Skin == 0x1F5E
		|| Skin == 0x2B0A
		|| Skin == 0x2AED
		|| Skin == 0x1D97
		|| Skin == 0x1F5F
		|| Skin == 0x1D1F
		|| Skin == 0x1F59
		|| Skin == 0x1D33
		|| Skin == 0x1F5C
		|| Skin == 0x2BD5
		|| Skin == 0x2C66
		|| Skin == 0x1D0B
		|| Skin == 0x1F58
		|| Skin == 0x1DD3
		|| Skin == 0x1F63
		|| Skin == 0x1DAB
		|| Skin == 0x1F61
		|| Skin == 0x1DBF
		|| Skin == 0x1F62
		|| Skin == 0x2C49
		|| Skin == 0x2C2C
		|| Skin == 0x2C0F
		|| Skin == 0x2BF2
		|| Skin == 0x2AD0
		|| Skin == 0x2AB5
		|| Skin == 0x2BB8
		|| Skin == 0x2B9B
		|| Skin == 0x2B7E
		|| Skin == 0x2B61
		|| Skin == 0x1D6F
		|| Skin == 0x1F5A
		|| Skin == 0x1C97
		|| Skin == 0x1F60
		|| Skin == 0x2B44
		|| Skin == 0x2B27
		|| Skin == 0x1D83
		|| Skin == 0x1F5B
		)
	{
		Hero = static_cast<Heroes>(13);
	}

	if (Skin == 0x1BE8
		|| Skin == 0x1C5E
		|| Skin == 0x1C5F
		|| Skin == 0x1C61
		|| Skin == 0x1C60
		|| Skin == 0x1C62
		|| Skin == 0x1C63
		|| Skin == 0x2136
		|| Skin == 0x1C6A
		|| Skin == 0x1C42
		|| Skin == 0x1C45
		|| Skin == 0x1C79
		|| Skin == 0x212A
		|| Skin == 0x1C75
		|| Skin == 0x2C72 // 여기까지 기본스킨
		|| Skin == 0x1D58
		|| Skin == 0x2052
		|| Skin == 0x1D6C
		|| Skin == 0x2053
		|| Skin == 0x2AF2
		|| Skin == 0x2AD5
		|| Skin == 0x1DA8
		|| Skin == 0x2054
		|| Skin == 0x1D30
		|| Skin == 0x204E
		|| Skin == 0x1D44
		|| Skin == 0x2051
		|| Skin == 0x2BBD
		|| Skin == 0x2C4E
		|| Skin == 0x1D1C
		|| Skin == 0x204D
		|| Skin == 0x1DE4
		|| Skin == 0x2058
		|| Skin == 0x1DBC
		|| Skin == 0x2056
		|| Skin == 0x1DD0
		|| Skin == 0x2057
		|| Skin == 0x2C31
		|| Skin == 0x2C14
		|| Skin == 0x2BF7
		|| Skin == 0x2BDA
		|| Skin == 0x2A9B
		|| Skin == 0x2A9D
		|| Skin == 0x2BA0
		|| Skin == 0x2B83
		|| Skin == 0x2B66
		|| Skin == 0x2B49
		|| Skin == 0x1D80
		|| Skin == 0x204F
		|| Skin == 0x1CAE
		|| Skin == 0x2055
		|| Skin == 0x2B2C
		|| Skin == 0x2B0F
		|| Skin == 0x1D94
		|| Skin == 0x2050
		)
	{
		Hero = static_cast<Heroes>(14);
	}

	if (Skin == 0x1933
		|| Skin == 0x1935
		|| Skin == 0x1934
		|| Skin == 0x1937
		|| Skin == 0x1936
		|| Skin == 0x1938
		|| Skin == 0x1B95
		|| Skin == 0x2A11
		|| Skin == 0x1C10
		|| Skin == 0x1B62
		|| Skin == 0x1B65
		|| Skin == 0x1B64
		|| Skin == 0x1B63
		|| Skin == 0x1B61
		|| Skin == 0x1C6F // 여기까지 기본스킨
		|| Skin == 0x1DF0
		|| Skin == 0x1F18
		|| Skin == 0x1DF3
		|| Skin == 0x1F19
		|| Skin == 0x2AF4
		|| Skin == 0x2AD7
		|| Skin == 0x1DFC
		|| Skin == 0x1F1A
		|| Skin == 0x1DEA
		|| Skin == 0x1F14
		|| Skin == 0x1DED
		|| Skin == 0x1F17
		|| Skin == 0x2BBF
		|| Skin == 0x2C50
		|| Skin == 0x1DE7
		|| Skin == 0x1F13
		|| Skin == 0x1E05
		|| Skin == 0x1F1E
		|| Skin == 0x1DFF
		|| Skin == 0x1F1C
		|| Skin == 0x1E02
		|| Skin == 0x1F1D
		|| Skin == 0x2C33
		|| Skin == 0x2C16
		|| Skin == 0x2BF9
		|| Skin == 0x2BDC
		|| Skin == 0x2ABA
		|| Skin == 0x2A9F
		|| Skin == 0x2BA2
		|| Skin == 0x2B85
		|| Skin == 0x2B68
		|| Skin == 0x2B4B
		|| Skin == 0x1DF6
		|| Skin == 0x1F15
		|| Skin == 0x1CAD
		|| Skin == 0x1F1B
		|| Skin == 0x2B2E
		|| Skin == 0x2B11
		|| Skin == 0x1DF9
		|| Skin == 0x1F16
		)
	{
		Hero = static_cast<Heroes>(15);
	}

	if (Skin == 0x16BF
		|| Skin == 0x18E1
		|| Skin == 0x16C0
		|| Skin == 0x16C1
		|| Skin == 0x16C2
		|| Skin == 0x1928
		|| Skin == 0x1929
		|| Skin == 0x19A0
		|| Skin == 0x1A16
		|| Skin == 0x2C96
		|| Skin == 0x19EA
		|| Skin == 0x1769
		|| Skin == 0x1768
		|| Skin == 0x192B
		|| Skin == 0x192A
		|| Skin == 0x20E6
		|| Skin == 0x1C0B
		|| Skin == 0x2A5C
		|| Skin == 0x19E6
		|| Skin == 0x16A7 // 여기까지 기본스킨
		|| Skin == 0x1CD5
		|| Skin == 0x1F94
		|| Skin == 0x1CD6
		|| Skin == 0x1F95
		|| Skin == 0x2B06
		|| Skin == 0x2AE9
		|| Skin == 0x1CD9
		|| Skin == 0x1F96
		|| Skin == 0x1CD3
		|| Skin == 0x1F90
		|| Skin == 0x1CD4
		|| Skin == 0x1F93
		|| Skin == 0x2BD1
		|| Skin == 0x2C62
		|| Skin == 0x1CD2
		|| Skin == 0x1F47
		|| Skin == 0x1CDC
		|| Skin == 0x1F9A
		|| Skin == 0x1CDA
		|| Skin == 0x1F98
		|| Skin == 0x1CDB
		|| Skin == 0x1F99
		|| Skin == 0x2C45
		|| Skin == 0x2C28
		|| Skin == 0x2C0B
		|| Skin == 0x2BEE
		|| Skin == 0x2ACC
		|| Skin == 0x2AB1
		|| Skin == 0x2BB4
		|| Skin == 0x2B97
		|| Skin == 0x2B7A
		|| Skin == 0x2B5D
		|| Skin == 0x1CD7
		|| Skin == 0x1F91
		|| Skin == 0x1C9B
		|| Skin == 0x1F97
		|| Skin == 0x2B40
		|| Skin == 0x2B23
		|| Skin == 0x1CD8
		|| Skin == 0x1F92
		)
	{
		Hero = static_cast<Heroes>(16);
	}

	if (Skin == 0x1697
		|| Skin == 0x1698
		|| Skin == 0x169A
		|| Skin == 0x18DB
		|| Skin == 0x1699
		|| Skin == 0x18DC
		|| Skin == 0x18DD
		|| Skin == 0x1C3C
		|| Skin == 0x1A07
		|| Skin == 0x1770
		|| Skin == 0x1771
		|| Skin == 0x167B
		|| Skin == 0x167C
		|| Skin == 0x1BC0
		|| Skin == 0x1BEB
		|| Skin == 0x195A
		|| Skin == 0x21C1
		|| Skin == 0x1C74
		|| Skin == 0x168B // 여기까지 기본스킨
		|| Skin == 0x1D45
		|| Skin == 0x1F3D
		|| Skin == 0x1D59
		|| Skin == 0x1F3E
		|| Skin == 0x2AF1
		|| Skin == 0x2AD4
		|| Skin == 0x1D95
		|| Skin == 0x1F3F
		|| Skin == 0x1D1D
		|| Skin == 0x1F39
		|| Skin == 0x1D31
		|| Skin == 0x1F3C
		|| Skin == 0x2BBC
		|| Skin == 0x2C4D
		|| Skin == 0x1D09
		|| Skin == 0x1F2D
		|| Skin == 0x1DD1
		|| Skin == 0x1F43
		|| Skin == 0x1DA9
		|| Skin == 0x1F41
		|| Skin == 0x1DBD
		|| Skin == 0x1F42
		|| Skin == 0x2C30
		|| Skin == 0x2C13
		|| Skin == 0x2BF6
		|| Skin == 0x2BD9
		|| Skin == 0x2A9A
		|| Skin == 0x2A9C
		|| Skin == 0x2B9F
		|| Skin == 0x2B82
		|| Skin == 0x2B65
		|| Skin == 0x2B48
		|| Skin == 0x1D6D
		|| Skin == 0x1F3A
		|| Skin == 0x1C95
		|| Skin == 0x1F40
		|| Skin == 0x2B2B
		|| Skin == 0x2B0E
		|| Skin == 0x1D81
		|| Skin == 0x1F3B
		)
	{
		Hero = static_cast<Heroes>(17);
	}

	if (Skin == 0x165F
		|| Skin == 0x1660
		|| Skin == 0x1662
		|| Skin == 0x18D6
		|| Skin == 0x163F
		|| Skin == 0x1954
		|| Skin == 0x18D5
		|| Skin == 0x1661
		|| Skin == 0x17B1
		|| Skin == 0x19A1
		|| Skin == 0x2A0C
		|| Skin == 0x1B87
		|| Skin == 0x1B7E
		|| Skin == 0x1640
		|| Skin == 0x163B
		|| Skin == 0x163C
		|| Skin == 0x1955
		|| Skin == 0x1EB6
		|| Skin == 0x21BB
		|| Skin == 0x1991 // 여기까지 기본스킨
		|| Skin == 0x1D49
		|| Skin == 0x1F87
		|| Skin == 0x1D5D
		|| Skin == 0x1F88
		|| Skin == 0x2B07
		|| Skin == 0x2AEA
		|| Skin == 0x1D99
		|| Skin == 0x1F89
		|| Skin == 0x1D21
		|| Skin == 0x1F83
		|| Skin == 0x1D35
		|| Skin == 0x1F86
		|| Skin == 0x2BD2
		|| Skin == 0x2C63
		|| Skin == 0x1D0D
		|| Skin == 0x1F82
		|| Skin == 0x1DD5
		|| Skin == 0x1F8D
		|| Skin == 0x1DAD
		|| Skin == 0x1F8B
		|| Skin == 0x1DC1
		|| Skin == 0x1F8C
		|| Skin == 0x2C46
		|| Skin == 0x2C29
		|| Skin == 0x2C0C
		|| Skin == 0x2BEF
		|| Skin == 0x2ACD
		|| Skin == 0x2AB2
		|| Skin == 0x2BB5
		|| Skin == 0x2B98
		|| Skin == 0x2B7B
		|| Skin == 0x2B5E
		|| Skin == 0x1D71
		|| Skin == 0x1F84
		|| Skin == 0x1C9A
		|| Skin == 0x1F8A
		|| Skin == 0x2B41
		|| Skin == 0x2B24
		|| Skin == 0x1D85
		|| Skin == 0x1F85
		)
	{
		Hero = static_cast<Heroes>(18);
	}

	if (Skin == 0x16A3
		|| Skin == 0x18DE
		|| Skin == 0x16A4
		|| Skin == 0x1A14
		|| Skin == 0x1C1C
		|| Skin == 0x16A5
		|| Skin == 0x16A6
		|| Skin == 0x18E0
		|| Skin == 0x18DF
		|| Skin == 0x1C1B
		|| Skin == 0x16A0
		|| Skin == 0x169F
		|| Skin == 0x169C
		|| Skin == 0x169B
		|| Skin == 0x20E9
		|| Skin == 0x195C
		|| Skin == 0x197D
		|| Skin == 0x2A0D // 여기까지 기본스킨
		|| Skin == 0x1D4F
		|| Skin == 0x207E
		|| Skin == 0x1D63
		|| Skin == 0x207F
		|| Skin == 0x2B0C
		|| Skin == 0x2AEF
		|| Skin == 0x1D9F
		|| Skin == 0x2080
		|| Skin == 0x1D27
		|| Skin == 0x207A
		|| Skin == 0x1D3B
		|| Skin == 0x207D
		|| Skin == 0x2BD7
		|| Skin == 0x2C68
		|| Skin == 0x1D13
		|| Skin == 0x2079
		|| Skin == 0x1DDB
		|| Skin == 0x2084
		|| Skin == 0x1DB3
		|| Skin == 0x2082
		|| Skin == 0x1DC7
		|| Skin == 0x2083
		|| Skin == 0x2C4B
		|| Skin == 0x2C2E
		|| Skin == 0x2C11
		|| Skin == 0x2BF4
		|| Skin == 0x2AD2
		|| Skin == 0x2AB7
		|| Skin == 0x2BBA
		|| Skin == 0x2B9D
		|| Skin == 0x2B80
		|| Skin == 0x2B63
		|| Skin == 0x1D77
		|| Skin == 0x207B
		|| Skin == 0x1CA2
		|| Skin == 0x2081
		|| Skin == 0x2B46
		|| Skin == 0x2B29
		|| Skin == 0x1D8B
		|| Skin == 0x207C
		)
	{
		Hero = static_cast<Heroes>(19);
	}

	if (Skin == 0x16C7
		|| Skin == 0x16C9
		|| Skin == 0x16CA
		|| Skin == 0x18E4
		|| Skin == 0x16C8
		|| Skin == 0x18E6
		|| Skin == 0x18E5
		|| Skin == 0x19A6
		|| Skin == 0x20E8
		|| Skin == 0x178C
		|| Skin == 0x178D
		|| Skin == 0x1791
		|| Skin == 0x1790
		|| Skin == 0x1BC8
		|| Skin == 0x1EBE
		|| Skin == 0x1C1E
		|| Skin == 0x1A1E
		|| Skin == 0x16C3
		|| Skin == 0x2A5D // 여기까지 기본스킨
		|| Skin == 0x1D52
		|| Skin == 0x20C5
		|| Skin == 0x1D66
		|| Skin == 0x20C6
		|| Skin == 0x2AFB
		|| Skin == 0x2ADE
		|| Skin == 0x1DA2
		|| Skin == 0x20C7
		|| Skin == 0x1D2A
		|| Skin == 0x20C1
		|| Skin == 0x1D3E
		|| Skin == 0x20C4
		|| Skin == 0x2BC6
		|| Skin == 0x2C57
		|| Skin == 0x1D16
		|| Skin == 0x20C0
		|| Skin == 0x1DDE
		|| Skin == 0x20CB
		|| Skin == 0x1DB6
		|| Skin == 0x20C9
		|| Skin == 0x1DCA
		|| Skin == 0x20CA
		|| Skin == 0x2C3A
		|| Skin == 0x2C1D
		|| Skin == 0x2C00
		|| Skin == 0x2BE3
		|| Skin == 0x2AC1
		|| Skin == 0x2AA6
		|| Skin == 0x2BA9
		|| Skin == 0x2B8C
		|| Skin == 0x2B6F
		|| Skin == 0x2B52
		|| Skin == 0x1D7A
		|| Skin == 0x20C2
		|| Skin == 0x1CA6
		|| Skin == 0x20C8
		|| Skin == 0x2B35
		|| Skin == 0x2B18
		|| Skin == 0x1D8E
		|| Skin == 0x20C3
		)
	{
		Hero = static_cast<Heroes>(20);
	}

	if (Skin == 0x162B
		|| Skin == 0x162D
		|| Skin == 0x162E
		|| Skin == 0x1A1D
		|| Skin == 0x162C
		|| Skin == 0x18CF
		|| Skin == 0x18D0
		|| Skin == 0x18D1
		|| Skin == 0x2138
		|| Skin == 0x198B
		|| Skin == 0x198A
		|| Skin == 0x198C
		|| Skin == 0x198D
		|| Skin == 0x1BC7
		|| Skin == 0x20DB
		|| Skin == 0x1C16
		|| Skin == 0x1C17
		|| Skin == 0x213B // 여기까지 기본스킨
		|| Skin == 0x1D56
		|| Skin == 0x203E
		|| Skin == 0x1D6A
		|| Skin == 0x203F
		|| Skin == 0x2AF7
		|| Skin == 0x2ADA
		|| Skin == 0x1DA6
		|| Skin == 0x2040
		|| Skin == 0x1D2E
		|| Skin == 0x203A
		|| Skin == 0x1D42
		|| Skin == 0x203D
		|| Skin == 0x2BC2
		|| Skin == 0x2C53
		|| Skin == 0x1D1A
		|| Skin == 0x2039
		|| Skin == 0x1DE2
		|| Skin == 0x2044
		|| Skin == 0x1DBA
		|| Skin == 0x2042
		|| Skin == 0x1DCE
		|| Skin == 0x2043
		|| Skin == 0x2C36
		|| Skin == 0x2C19
		|| Skin == 0x2BFC
		|| Skin == 0x2BDF
		|| Skin == 0x2ABD
		|| Skin == 0x2AA2
		|| Skin == 0x2BA5
		|| Skin == 0x2B88
		|| Skin == 0x2B6B
		|| Skin == 0x2B4E
		|| Skin == 0x1D7E
		|| Skin == 0x203B
		|| Skin == 0x1CAA
		|| Skin == 0x2041
		|| Skin == 0x2B31
		|| Skin == 0x2B14
		|| Skin == 0x1D92
		|| Skin == 0x203C
		)
	{
		Hero = static_cast<Heroes>(21);
	}

	if (Skin == 0x16E7
		|| Skin == 0x16E9
		|| Skin == 0x1998
		|| Skin == 0x16E4
		|| Skin == 0x16E3
		|| Skin == 0x16DB
		|| Skin == 0x16E8
		|| Skin == 0x16EA
		|| Skin == 0x18ED
		|| Skin == 0x1C44
		|| Skin == 0x18EE
		|| Skin == 0x18EF
		|| Skin == 0x1A27
		|| Skin == 0x16DC
		|| Skin == 0x16DF
		|| Skin == 0x2133
		|| Skin == 0x1E0A
		|| Skin == 0x1B92 // 여기까지 기본스킨
		|| Skin == 0x1D4D
		|| Skin == 0x20D2
		|| Skin == 0x1D61
		|| Skin == 0x20D3
		|| Skin == 0x2B02
		|| Skin == 0x2AE5
		|| Skin == 0x1D9D
		|| Skin == 0x20D4
		|| Skin == 0x1D25
		|| Skin == 0x20CE
		|| Skin == 0x1D39
		|| Skin == 0x20D1
		|| Skin == 0x2BCD
		|| Skin == 0x2C5E
		|| Skin == 0x1D11
		|| Skin == 0x20CD
		|| Skin == 0x1DD9
		|| Skin == 0x20D8
		|| Skin == 0x1DB1
		|| Skin == 0x20D6
		|| Skin == 0x1DC5
		|| Skin == 0x20D7
		|| Skin == 0x2C41
		|| Skin == 0x2C24
		|| Skin == 0x2C07
		|| Skin == 0x2BEA
		|| Skin == 0x2AC8
		|| Skin == 0x2AAD
		|| Skin == 0x2BB0
		|| Skin == 0x2B93
		|| Skin == 0x2B76
		|| Skin == 0x2B59
		|| Skin == 0x1D75
		|| Skin == 0x20CF
		|| Skin == 0x1C9F
		|| Skin == 0x20D5
		|| Skin == 0x2B3C
		|| Skin == 0x2B1F
		|| Skin == 0x1D89
		|| Skin == 0x20D0
		)
	{
		Hero = static_cast<Heroes>(22);
	}

	if (Skin == 0x1637
		|| Skin == 0x1638
		|| Skin == 0x1639
		|| Skin == 0x1BAD
		|| Skin == 0x163A
		|| Skin == 0x18D2
		|| Skin == 0x18D4
		|| Skin == 0x18D3
		|| Skin == 0x197B
		|| Skin == 0x1C18
		|| Skin == 0x162F
		|| Skin == 0x1630
		|| Skin == 0x1633
		|| Skin == 0x1634
		|| Skin == 0x1BCB
		|| Skin == 0x1968
		|| Skin == 0x21AB
		|| Skin == 0x1B60
		|| Skin == 0x1C6E
		|| Skin == 0x2A56 // 여기까지 기본스킨
		|| Skin == 0x1D48
		|| Skin == 0x1F78
		|| Skin == 0x1D5C
		|| Skin == 0x1F79
		|| Skin == 0x2B08
		|| Skin == 0x2AEB
		|| Skin == 0x1D98
		|| Skin == 0x1F7A
		|| Skin == 0x1D20
		|| Skin == 0x1F74
		|| Skin == 0x1D34
		|| Skin == 0x1F77
		|| Skin == 0x2BD3
		|| Skin == 0x2C64
		|| Skin == 0x1D0C
		|| Skin == 0x1F73
		|| Skin == 0x1DD4
		|| Skin == 0x1F7E
		|| Skin == 0x1DAC
		|| Skin == 0x1F7C
		|| Skin == 0x1DC0
		|| Skin == 0x1F7D
		|| Skin == 0x2C47
		|| Skin == 0x2C2A
		|| Skin == 0x2C0D
		|| Skin == 0x2BF0
		|| Skin == 0x2ACE
		|| Skin == 0x2AB3
		|| Skin == 0x2BB6
		|| Skin == 0x2B99
		|| Skin == 0x2B7C
		|| Skin == 0x2B5F
		|| Skin == 0x1D70
		|| Skin == 0x1F75
		|| Skin == 0x1C99
		|| Skin == 0x1F7B
		|| Skin == 0x2B42
		|| Skin == 0x2B25
		|| Skin == 0x1D84
		|| Skin == 0x1F76
		)
	{
		Hero = static_cast<Heroes>(23);

	}

	if (Skin == 0x170F
		|| Skin == 0x1711
		|| Skin == 0x1712
		|| Skin == 0x1710
		|| Skin == 0x18F1
		|| Skin == 0x1942
		|| Skin == 0x195E
		|| Skin == 0x18F0
		|| Skin == 0x18F2
		|| Skin == 0x1B07
		|| Skin == 0x20EB
		|| Skin == 0x16EB
		|| Skin == 0x16EC
		|| Skin == 0x170B
		|| Skin == 0x170C
		|| Skin == 0x1943
		|| Skin == 0x21F2
		|| Skin == 0x1B66
		|| Skin == 0x1B2B
		|| Skin == 0x16F7 // 여기까지 기본스킨
		|| Skin == 0x1D46
		|| Skin == 0x1F4C
		|| Skin == 0x1D5A
		|| Skin == 0x1F4D
		|| Skin == 0x2B0B
		|| Skin == 0x2AEE
		|| Skin == 0x1D96
		|| Skin == 0x1F4E
		|| Skin == 0x1D1E
		|| Skin == 0x1F48
		|| Skin == 0x1D32
		|| Skin == 0x1F4B
		|| Skin == 0x2BD6
		|| Skin == 0x2C67
		|| Skin == 0x1D0A
		|| Skin == 0x1F47
		|| Skin == 0x1DD2
		|| Skin == 0x1F52
		|| Skin == 0x1DAA
		|| Skin == 0x1F50
		|| Skin == 0x1DBE
		|| Skin == 0x1F51
		|| Skin == 0x2C4A
		|| Skin == 0x2C2D
		|| Skin == 0x2C10
		|| Skin == 0x2BF3
		|| Skin == 0x2AD1
		|| Skin == 0x2AB6
		|| Skin == 0x2BB9
		|| Skin == 0x2B9C
		|| Skin == 0x2B7F
		|| Skin == 0x2B62
		|| Skin == 0x1D6E
		|| Skin == 0x1F49
		|| Skin == 0x1C96
		|| Skin == 0x1F4F
		|| Skin == 0x2B45
		|| Skin == 0x2B28
		|| Skin == 0x1D82
		|| Skin == 0x1F4A
		)
	{
		//TRACER
		Hero = static_cast<Heroes>(24);
	}

	if (Skin == 0x171B
		|| Skin == 0x171C
		|| Skin == 0x171E
		|| Skin == 0x18F3
		|| Skin == 0x171D
		|| Skin == 0x1C21
		|| Skin == 0x18F4
		|| Skin == 0x18F5
		|| Skin == 0x1978
		|| Skin == 0x21BA
		|| Skin == 0x1714
		|| Skin == 0x1713
		|| Skin == 0x1919
		|| Skin == 0x1918
		|| Skin == 0x1C20
		|| Skin == 0x1BC9
		|| Skin == 0x2113
		|| Skin == 0x1C1F
		|| Skin == 0x1BAF
		|| Skin == 0x1DC3
		|| Skin == 0x2B3E
		|| Skin == 0x1D9B
		|| Skin == 0x1F2A
		|| Skin == 0x2CEE
		|| Skin == 0x1D73
		|| Skin == 0x2BB2
		|| Skin == 0x1F27
		|| Skin == 0x1717
		|| Skin == 0x1D87
		|| Skin == 0x1DD7
		|| Skin == 0x1E54
		|| Skin == 0x1F2B
		)
	{
		//WIDOWMAKER
		Hero = static_cast<Heroes>(25);
	}

	if (Skin == 0x16D3
		|| Skin == 0x16D6
		|| Skin == 0x18E7
		|| Skin == 0x18E9
		|| Skin == 0x197F
		|| Skin == 0x16D5
		|| Skin == 0x16D4
		|| Skin == 0x18E8
		|| Skin == 0x16CF
		|| Skin == 0x16D0
		|| Skin == 0x16CB
		|| Skin == 0x16CC
		|| Skin == 0x1EAC
		|| Skin == 0x1964
		|| Skin == 0x1CC6
		|| Skin == 0x2A4F // 여기까지 기본스킨
		|| Skin == 0x1D4A
		|| Skin == 0x208B
		|| Skin == 0x1D5E
		|| Skin == 0x208C
		|| Skin == 0x2B05
		|| Skin == 0x2AE8
		|| Skin == 0x1D9A
		|| Skin == 0x208D
		|| Skin == 0x1D22
		|| Skin == 0x2087
		|| Skin == 0x1D36
		|| Skin == 0x208A
		|| Skin == 0x2BD0
		|| Skin == 0x2C61
		|| Skin == 0x1D0E
		|| Skin == 0x2086
		|| Skin == 0x1DD6
		|| Skin == 0x2091
		|| Skin == 0x1DAE
		|| Skin == 0x208F
		|| Skin == 0x1DC2
		|| Skin == 0x2090
		|| Skin == 0x2C44
		|| Skin == 0x2C27
		|| Skin == 0x2C0A
		|| Skin == 0x2BED
		|| Skin == 0x2ACB
		|| Skin == 0x2AB0
		|| Skin == 0x2BB3
		|| Skin == 0x2B96
		|| Skin == 0x2B79
		|| Skin == 0x2B5C
		|| Skin == 0x1D72
		|| Skin == 0x2088
		|| Skin == 0x1C9C
		|| Skin == 0x208E
		|| Skin == 0x2B3F
		|| Skin == 0x2B22
		|| Skin == 0x1D86
		|| Skin == 0x2089
		)
	{
		//WINSTON
		Hero = static_cast<Heroes>(26);
	}

	if (Skin == 0x1C56
		|| Skin == 0x213F
		|| Skin == 0x213D
		|| Skin == 0x213C
		|| Skin == 0x213E
		|| Skin == 0x2143
		|| Skin == 0x2142
		|| Skin == 0x214A
		|| Skin == 0x2128
		|| Skin == 0x2147
		|| Skin == 0x2130
		|| Skin == 0x214E
		|| Skin == 0x2CD8
		|| Skin == 0x2A68 // 여기까지 기본스킨
		|| Skin == 0x218C
		|| Skin == 0x219B
		|| Skin == 0x218D
		|| Skin == 0x219C
		|| Skin == 0x2AFE
		|| Skin == 0x2AE1
		|| Skin == 0x2190
		|| Skin == 0x219D
		|| Skin == 0x218A
		|| Skin == 0x2197
		|| Skin == 0x218B
		|| Skin == 0x219A
		|| Skin == 0x2BC9
		|| Skin == 0x2C5A
		|| Skin == 0x2189
		|| Skin == 0x2196
		|| Skin == 0x2194
		|| Skin == 0x21A1
		|| Skin == 0x2192
		|| Skin == 0x219F
		|| Skin == 0x2193
		|| Skin == 0x21A0
		|| Skin == 0x2C3D
		|| Skin == 0x2C20
		|| Skin == 0x2C03
		|| Skin == 0x2BE6
		|| Skin == 0x2AC4
		|| Skin == 0x2AA9
		|| Skin == 0x2BAC
		|| Skin == 0x2B8F
		|| Skin == 0x2B72
		|| Skin == 0x2B55
		|| Skin == 0x218E
		|| Skin == 0x2198
		|| Skin == 0x2191
		|| Skin == 0x219E
		|| Skin == 0x2B38
		|| Skin == 0x2B1B
		|| Skin == 0x218F
		|| Skin == 0x2199
		)
	{
		//WRECKINGBALL
		Hero = static_cast<Heroes>(27);
	}

	if (Skin == 0x166B
		|| Skin == 0x166E
		|| Skin == 0x18D8
		|| Skin == 0x166D
		|| Skin == 0x166C
		|| Skin == 0x18D9
		|| Skin == 0x1A1B
		|| Skin == 0x1664
		|| Skin == 0x18DA
		|| Skin == 0x1C1A
		|| Skin == 0x1668
		|| Skin == 0x1667
		|| Skin == 0x1663
		|| Skin == 0x1944
		|| Skin == 0x1945
		|| Skin == 0x196E
		|| Skin == 0x2134
		|| Skin == 0x1C19
		|| Skin == 0x1B6A
		|| Skin == 0x2CDF // 여기까지 기본스킨
		|| Skin == 0x1D51
		|| Skin == 0x2070
		|| Skin == 0x1D65
		|| Skin == 0x2071
		|| Skin == 0x2AFC
		|| Skin == 0x2ADF
		|| Skin == 0x1DA1
		|| Skin == 0x2072
		|| Skin == 0x1D29
		|| Skin == 0x206C
		|| Skin == 0x1D3D
		|| Skin == 0x206F
		|| Skin == 0x2BC7
		|| Skin == 0x2C58
		|| Skin == 0x1D15
		|| Skin == 0x206B
		|| Skin == 0x1DDD
		|| Skin == 0x2076
		|| Skin == 0x1DB5
		|| Skin == 0x2074
		|| Skin == 0x1DC9
		|| Skin == 0x2075
		|| Skin == 0x2C3B
		|| Skin == 0x2C1E
		|| Skin == 0x2C01
		|| Skin == 0x2BE4
		|| Skin == 0x2AC2
		|| Skin == 0x2AA7
		|| Skin == 0x2BAA
		|| Skin == 0x2B8D
		|| Skin == 0x2B70
		|| Skin == 0x2B53
		|| Skin == 0x1D79
		|| Skin == 0x206D
		|| Skin == 0x1CA5
		|| Skin == 0x2073
		|| Skin == 0x2B36
		|| Skin == 0x2B19
		|| Skin == 0x1D8D
		|| Skin == 0x206E
		)
	{
		//ZARYA
		Hero = static_cast<Heroes>(28);
	}

	if (Skin == 0x1603
		|| Skin == 0x18BA
		|| Skin == 0x1604
		|| Skin == 0x18BB
		|| Skin == 0x15FB
		|| Skin == 0x1966
		|| Skin == 0x1605
		|| Skin == 0x1606
		|| Skin == 0x1C40
		|| Skin == 0x18BC
		|| Skin == 0x19A3
		|| Skin == 0x15FC
		|| Skin == 0x15FF
		|| Skin == 0x1600
		|| Skin == 0x1E2D
		|| Skin == 0x1BE7
		|| Skin == 0x1960
		|| Skin == 0x21D1 // 여기까지 기본스킨
		|| Skin == 0x1D4E
		|| Skin == 0x1FDE
		|| Skin == 0x1D62
		|| Skin == 0x1FDF
		|| Skin == 0x2B01
		|| Skin == 0x2AE4
		|| Skin == 0x1D9E
		|| Skin == 0x1FE0
		|| Skin == 0x1D26
		|| Skin == 0x1FDA
		|| Skin == 0x1D3A
		|| Skin == 0x1FDD
		|| Skin == 0x2BCC
		|| Skin == 0x2C5D
		|| Skin == 0x1D12
		|| Skin == 0x1FD9
		|| Skin == 0x1DDA
		|| Skin == 0x1FE4
		|| Skin == 0x1DB2
		|| Skin == 0x1FE2
		|| Skin == 0x1DC6
		|| Skin == 0x1FE3
		|| Skin == 0x2C40
		|| Skin == 0x2C23
		|| Skin == 0x2C06
		|| Skin == 0x2BE9
		|| Skin == 0x2AC7
		|| Skin == 0x2AAC
		|| Skin == 0x2BAF
		|| Skin == 0x2B92
		|| Skin == 0x2B75
		|| Skin == 0x2B58
		|| Skin == 0x1D76
		|| Skin == 0x1FDB
		|| Skin == 0x1CA0
		|| Skin == 0x1FE1
		|| Skin == 0x2B3B
		|| Skin == 0x2B1E
		|| Skin == 0x1D8A
		|| Skin == 0x1FDC
		)
	{
		//ZENYATTA
		Hero = static_cast<Heroes>(29);
	}

	if (Skin == 0x28CF
		|| Skin == 0x2D65
		|| Skin == 0x2D63
		|| Skin == 0x2D64
		|| Skin == 0x2D62
		|| Skin == 0x2D66
		|| Skin == 0x2D67
		|| Skin == 0x2D26
		|| Skin == 0x2D18
		|| Skin == 0x2D16
		|| Skin == 0x2D70 // 여기까지 기본스킨
		|| Skin == 0x2D43
		|| Skin == 0x2D44
		|| Skin == 0x2D45
		|| Skin == 0x2D46
		|| Skin == 0x2D3B
		|| Skin == 0x2D3C
		|| Skin == 0x2D47
		|| Skin == 0x2D2E
		|| Skin == 0x2D37
		|| Skin == 0x2D38
		|| Skin == 0x2D41
		|| Skin == 0x2D42
		|| Skin == 0x2D54
		|| Skin == 0x2D55
		|| Skin == 0x2D31
		|| Skin == 0x2D32
		|| Skin == 0x2D50
		|| Skin == 0x2D51
		|| Skin == 0x2D4C
		|| Skin == 0x2D4D
		|| Skin == 0x2D4E
		|| Skin == 0x2D4F
		|| Skin == 0x2D2F
		|| Skin == 0x2D30
		|| Skin == 0x2D56
		|| Skin == 0x2D57
		|| Skin == 0x2D33
		|| Skin == 0x2D34
		|| Skin == 0x2D52
		|| Skin == 0x2D53
		|| Skin == 0x2D48
		|| Skin == 0x2D49
		|| Skin == 0x2D39
		|| Skin == 0x2D3A
		|| Skin == 0x2D4A
		|| Skin == 0x2D4B
		|| Skin == 0x2D3D
		|| Skin == 0x2D3E
		|| Skin == 0x2D3F
		|| Skin == 0x2D40
		)
	{
		//SIGMA
		Hero = static_cast<Heroes>(30);
	}
	if (Skin == 0x20E0 ||
		Skin == 0x46C3 ||
		Skin == 0x46C6 ||
		Skin == 0x46C4 ||
		Skin == 0x46C5 ||
		Skin == 0x46C7 ||
		Skin == 0x46C8 ||
		Skin == 0x46B7 ||
		Skin == 0x46E4 ||
		Skin == 0x46BC ||
		Skin == 0x46D4 ||
		Skin == 0x4718 ||
		Skin == 0x4719 ||
		Skin == 0x471D ||
		Skin == 0x471C ||
		Skin == 0x4710 ||
		Skin == 0x4711 ||
		Skin == 0x471E ||
		Skin == 0x471F ||
		Skin == 0x470A ||
		Skin == 0x470B ||
		Skin == 0x4716 ||
		Skin == 0x4717 ||
		Skin == 0x472E ||
		Skin == 0x472F ||
		Skin == 0x4704 ||
		Skin == 0x4705 ||
		Skin == 0x472A ||
		Skin == 0x472B ||
		Skin == 0x4727 ||
		Skin == 0x4726 ||
		Skin == 0x4728 ||
		Skin == 0x4729 ||
		Skin == 0x4702 ||
		Skin == 0x4703 ||
		Skin == 0x4730 ||
		Skin == 0x4731 ||
		Skin == 0x4706 ||
		Skin == 0x4707 ||
		Skin == 0x472C ||
		Skin == 0x472D ||
		Skin == 0x4720 ||
		Skin == 0x4721 ||
		Skin == 0x470F ||
		Skin == 0x470E ||
		Skin == 0x4722 ||
		Skin == 0x4723 ||
		Skin == 0x4712 ||
		Skin == 0x4713 ||
		Skin == 0x4714 ||
		Skin == 0x4715)
	{
		//ECHO
		Hero = static_cast<Heroes>(31);
	}
	if (
		Skin == 0x1AFC
		|| Skin == 0x1AF8
		|| Skin == 0x1AFA
		)
	{
		//TRAINING_BOT
		Hero = static_cast<Heroes>(32);
	}

	return Hero;
}

Heroes GUID2Hero(DWORD64 GUID)
{
	switch (GUID)
	{
	case 0x2E000000000013B: return ANA;
	case 0x2E0000000000200: return ASHE;
	case 0x2E0000000000221: return BARTIST;
	case 0x2E0000000000015: return BASTION;
	case 0x2E0000000000195: return BRIGITTE;
	case 0x2E000000000012F: return DOOMFIST;
	case 0x2E000000000007A: return DVA;
	case 0x2E0000000000029: return GENJI;
	case 0x2E0000000000005: return HANZO;
	case 0x2E0000000000065: return JUNKRAT;
	case 0x2E0000000000079: return LUCIO;
	case 0x2E0000000000042: return MCCREE;
	case 0x2E00000000000DD: return MEI;
	case 0x2E0000000000004: return MERCY;
	case 0x2E00000000001A2: return MOIRA;
	case 0x2E000000000013E: return ORISA;
	case 0x2E0000000000008: return PHARAH;
	case 0x2E0000000000002: return REAPER;
	case 0x2E0000000000007: return REINHARDT;
	case 0x2E0000000000040: return ROADHOG;
	case 0x2E000000000006E: return SOLDIER;
	case 0x2E000000000012E: return SOMBRA;
	case 0x2E0000000000016: return SYMMETRA;
	case 0x2E0000000000006: return TORBJORN;
	case 0x2E0000000000003: return TRACER;
	case 0x2E000000000000A: return WIDOWMAKER;
	case 0x2E0000000000009: return WINSTON;
	case 0x2E00000000001CA: return WRECKINGBALL;
	case 0x2E0000000000068: return ZARYA;
	case 0x2E0000000000020: return ZENYATTA;
	case 0x2E000000000023B: return SIGMA;
	case 0x02E0000000000206: return ECHO;
	}

	return XXX;
}