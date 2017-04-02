                 ADD                   Rn                   Rm  1000_nnnn_mmmm_0001                      83D1
                 ADD                   Rn                #imm8  0001_nnnn_iiii_iiii                      1A7F
                 ADD                  ERn                  ERm  1111_nnn0_mmm0_0110                      F286
                 ADD                  ERn                #imm7  1110_nnn0_1iii_iiii                      E2FB
                ADDC                   Rn                   Rm  1000_nnnn_mmmm_0110                      8066
                ADDC                   Rn                #imm8  0110_nnnn_iiii_iiii                      6FED
                 AND                   Rn                   Rm  1000_nnnn_mmmm_0010                      8672
                 AND                   Rn                #imm8  0010_nnnn_iiii_iiii                      2DAE
                 CMP                   Rn                   Rm  1000_nnnn_mmmm_0111                      80C7
                 CMP                   Rn                #imm8  0111_nnnn_iiii_iiii                      7343
                CMPC                   Rn                   Rm  1000_nnnn_mmmm_0101                      88D5
                CMPC                   Rn                #imm8  0101_nnnn_iiii_iiii                      53BE
                 MOV                  ERn                  ERm  1111_nnn0_mmm0_0101                      FC45
                 MOV                  ERn                #imm7  1110_nnn0_0iii_iiii                      E45C
                 MOV                   Rn                   Rm  1000_nnnn_mmmm_0000                      8090
                 MOV                   Rn                #imm8  0000_nnnn_iiii_iiii                      0E26
                  OR                   Rn                   Rm  1000_nnnn_mmmm_0011                      8453
                  OR                   Rn                #imm8  0011_nnnn_iiii_iiii                      338C
                 XOR                   Rn                   Rm  1000_nnnn_mmmm_0100                      8294
                 XOR                   Rn                #imm8  0100_nnnn_iiii_iiii                      4FCD
                 CMP                  ERn                  ERm  1111_nnn0_mmm0_0111                      F4E7
                 SUB                   Rn                   Rm  1000_nnnn_mmmm_1000                      8448
                SUBC                   Rn                   Rm  1000_nnnn_mmmm_1001                      8F59
                 SLL                   Rn                   Rm  1000_nnnn_mmmm_1010                      835A
                 SLL                   Rn               #width  1001_nnnn_0www_1010                      9A7A
                SLLC                   Rn                   Rm  1000_nnnn_mmmm_1011                      805B
                SLLC                   Rn               #width  1001_nnnn_0www_1011                      975B
                 SRA                   Rn                   Rm  1000_nnnn_mmmm_1110                      8A5E
                 SRA                   Rn               #width  1001_nnnn_0www_1110                      9D2E
                 SRL                   Rn                   Rm  1000_nnnn_mmmm_1100                      801C
                 SRL                   Rn               #width  1001_nnnn_0www_1100                      975C
                SRLC                   Rn                   Rm  1000_nnnn_mmmm_1101                      810D
                SRLC                   Rn               #width  1001_nnnn_0www_1101                      9A1D
                   L                  ERn                 [EA]  1001_nnn0_0011_0010                      9A32
                   L                  ERn                [EA+]  1001_nnn0_0101_0010                      9252
                   L                  ERn                [ERm]  1001_nnn0_mmm0_0010                      9AE2
                   L                  ERn          Disp16[ERm]  1010_nnn0_mmm0_1000  DDDD_DDDD_DDDD_DDDD AA48 19F2
                   L                  ERn            Disp6[BP]  1011_nnn0_00DD_DDDD                      B823
                   L                  ERn            Disp6[FP]  1011_nnn0_01DD_DDDD                      BC55
                   L                  ERn                 Dadr  1001_nnn0_0001_0010  DDDD_DDDD_DDDD_DDDD 9C12 E183
                   L                   Rn                 [EA]  1001_nnnn_0011_0000                      9F30
                   L                   Rn                [EA+]  1001_nnnn_0101_0000                      9A50
                   L                   Rn                [ERm]  1001_nnnn_mmm0_0000                      9640
                   L                   Rn          Disp16[ERm]  1001_nnnn_mmm0_1000  DDDD_DDDD_DDDD_DDDD 9068 98BB
                   L                   Rn            Disp6[BP]  1101_nnnn_00DD_DDDD                      DA32
                   L                   Rn            Disp6[FP]  1101_nnnn_01DD_DDDD                      D37E
                   L                   Rn                 Dadr  1001_nnnn_0001_0000  DDDD_DDDD_DDDD_DDDD 9B10 AA80
                   L                  XRn                 [EA]  1001_nn00_0011_0100                      9834
                   L                  XRn                [EA+]  1001_nn00_0101_0100                      9C54
                   L                  QRn                 [EA]  1001_n000_0011_0110                      9836
                   L                  QRn                [EA+]  1001_n000_0101_0110                      9856
                  ST                  ERn                 [EA]  1001_nnn0_0011_0011                      9A33
                  ST                  ERn                [EA+]  1001_nnn0_0101_0011                      9A53
                  ST                  ERn                [ERm]  1001_nnn0_mmm0_0011                      94A3
                  ST                  ERn          Disp16[ERm]  1010_nnn0_mmm0_1001  DDDD_DDDD_DDDD_DDDD A469 2759
                  ST                  ERn            Disp6[BP]  1011_nnn0_10DD_DDDD                      B4B4
                  ST                  ERn            Disp6[FP]  1011_nnn0_11DD_DDDD                      BCFA
                  ST                  ERn                 Dadr  1001_nnn0_0001_0011  DDDD_DDDD_DDDD_DDDD 9813 8F45
                  ST                   Rn                 [EA]  1001_nnnn_0011_0001                      9731
                  ST                   Rn                [EA+]  1001_nnnn_0101_0001                      9B51
                  ST                   Rn                [ERm]  1001_nnnn_mmm0_0001                      9A21
                  ST                   Rn          Disp16[ERm]  1001_nnnn_mmm0_1001  DDDD_DDDD_DDDD_DDDD 92E9 E5D0
                  ST                   Rn            Disp6[BP]  1101_nnnn_10DD_DDDD                      D29E
                  ST                   Rn            Disp6[FP]  1101_nnnn_11DD_DDDD                      DFCA
                  ST                   Rn                 Dadr  1001_nnnn_0001_0001  DDDD_DDDD_DDDD_DDDD 9311 8462
                  ST                  XRn                 [EA]  1001_nn00_0011_0101                      9C35
                  ST                  XRn                [EA+]  1001_nn00_0101_0101                      9C55
                  ST                  QRn                 [EA]  1001_n000_0011_0111                      9837
                  ST                  QRn                [EA+]  1001_n000_0101_0111                      9857
                 ADD                   SP             #signed8  1110_0001_iiii_iiii                      E157
                 MOV                 ECSR                   Rm  1010_0000_mmmm_1111                      A07F
                 MOV                  ELR                  ERm  1010_mmm0_0000_1101                      A20D
                 MOV                 EPSW                   Rm  1010_0000_mmmm_1100                      A0AC
                 MOV                  ERn                  ELR  1010_nnn0_0000_0101                      AE05
                 MOV                  ERn                   SP  1010_nnn0_0001_1010                      AC1A
                 MOV                  PSW                   Rm  1010_0000_mmmm_1011                      A04B
                 MOV                  PSW           #unsigned8  1110_1001_iiii_iiii                      E9CA
                 MOV                   Rn                 ECSR  1010_nnnn_0000_0111                      AA07
                 MOV                   Rn                 EPSW  1010_nnnn_0000_0100                      AB04
                 MOV                   Rn                  PSW  1010_nnnn_0000_0011                      A303
                 MOV                   SP                  ERm  1010_0001_mmm0_1010                      A12A
                PUSH                  ERn                       1111_nnn0_0101_1110                      FE5E
                PUSH                  QRn                       1111_n000_0111_1110                      F87E
                PUSH                   Rn                       1111_nnnn_0100_1110                      F34E
                PUSH                  XRn                       1111_nn00_0110_1110                      F86E
                PUSH        register_list                       1111_lepa_1100_1110                      F3CE
                 POP                  ERn                       1111_nnn0_0001_1110                      F21E
                 POP                  QRn                       1111_n000_0011_1110                      F03E
                 POP                   Rn                       1111_nnnn_0000_1110                      FF0E
                 POP                  XRn                       1111_nn00_0010_1110                      FC2E
                 POP        register_list                       1111_lepa_1000_1110                      F68E
                 MOV                  CRn                   Rm  1010_nnnn_mmmm_1110                      A56E
                 MOV                 CERn                 [EA]  1111_nnn0_0010_1101                      F02D
                 MOV                 CERn                [EA+]  1111_nnn0_0011_1101                      F43D
                 MOV                  CRn                 [EA]  1111_nnnn_0000_1101                      F80D
                 MOV                  CRn                [EA+]  1111_nnnn_0001_1101                      F41D
                 MOV                 CXRn                 [EA]  1111_nn00_0100_1101                      F44D
                 MOV                 CXRn                [EA+]  1111_nn00_0101_1101                      F45D
                 MOV                 CQRn                 [EA]  1111_n000_0110_1101                      F86D
                 MOV                 CQRn                [EA+]  1111_n000_0111_1101                      F07D
                 MOV                   Rn                  CRm  1010_nnnn_mmmm_0110                      AC56
                 MOV                 [EA]                 CERm  1111_mmm0_1010_1101                      F4AD
                 MOV                [EA+]                 CERm  1111_mmm0_1011_1101                      F6BD
                 MOV                 [EA]                  CRm  1111_mmmm_1000_1101                      FE8D
                 MOV                [EA+]                  CRm  1111_mmmm_1001_1101                      F49D
                 MOV                 [EA]                 CXRm  1111_mm00_1100_1101                      F4CD
                 MOV                [EA+]                 CXRm  1111_mm00_1101_1101                      FCDD
                 MOV                 [EA]                 CQRm  1111_m000_1110_1101                      F8ED
                 MOV                [EA+]                 CQRm  1111_m000_1111_1101                      F8FD
                 LEA                [ERm]                       1111_0000_mmm0_1010                      F08A
                 LEA          Disp16[ERm]                       1111_0000_mmm0_1011  DDDD_DDDD_DDDD_DDDD F08B EE34
                 LEA                 Dadr                       1111_0000_0000_1100  DDDD_DDDD_DDDD_DDDD F00C 7DF1
                 DAA                   Rn                       1000_nnnn_0001_1111                      821F
                 DAS                   Rn                       1000_nnnn_0011_1111                      863F
                 NEG                   Rn                       1000_nnnn_0101_1111                      815F
                  SB        Rn.bit_offset                       1010_nnnn_0bbb_0000                      A550
                  SB              Dbitadr                       1010_0000_1bbb_0000  DDDD_DDDD_DDDD_DDDD A0C0 B132
                  RB       Rn. bit_offset                       1010_nnnn_0bbb_0010                      A252
                  RB              Dbitadr                       1010_0000_1bbb_0010  DDDD_DDDD_DDDD_DDDD A0D2 9692
                  TB       Rn. bit_offset                       1010_nnnn_0bbb_0001                      A321
                  TB              Dbitadr                       1010_0000_1bbb_0001  DDDD_DDDD_DDDD_DDDD A0E1 FEF9
                  EI                                            1110_1101_0000_1000                      ED08
                  DI                                            1110_1011_1111_0111                      EBF7
                  SC                                            1110_1101_1000_0000                      ED80
                  RC                                            1110_1011_0111_1111                      EB7F
                CPLC                                            1111_1110_1100_1111                      FECF
                 BGE                 Radr                       1100_0000_rrrr_rrrr                      C055
                 BLT                 Radr                       1100_0001_rrrr_rrrr                      C12F
                 BGT                 Radr                       1100_0010_rrrr_rrrr                      C228
                 BLE                 Radr                       1100_0011_rrrr_rrrr                      C359
                BGES                 Radr                       1100_0100_rrrr_rrrr                      C40B
                BLTS                 Radr                       1100_0101_rrrr_rrrr                      C583
                BGTS                 Radr                       1100_0110_rrrr_rrrr                      C69E
                BLES                 Radr                       1100_0111_rrrr_rrrr                      C7B2
                 BNE                 Radr                       1100_1000_rrrr_rrrr                      C87A
                 BEQ                 Radr                       1100_1001_rrrr_rrrr                      C937
                 BNV                 Radr                       1100_1010_rrrr_rrrr                      CA51
                 BOV                 Radr                       1100_1011_rrrr_rrrr                      CBAF
                 BPS                 Radr                       1100_1100_rrrr_rrrr                      CC85
                 BNS                 Radr                       1100_1101_rrrr_rrrr                      CD42
                 BAL                 Radr                       1100_1110_rrrr_rrrr                      CED6
               EXTBW                  ERn                       1000_nnn1_nnn0_1111                      89CF
                 SWI                #snum                       1110_0101_00ii_iiii                      E524
                 BRK                                            1111_1111_1111_1111                      FFFF
                   B                 Cadr                       1111_gggg_0000_0000  CCCC_CCCC_CCCC_CCCC F300 E316
                   B                  ERn                       1111_0000_nnn0_0010                      F002
                  BL                 Cadr                       1111_gggg_0000_0001  CCCC_CCCC_CCCC_CCCC F301 79A9
                  BL                  ERn                       1111_0000_nnn0_0011                      F023
                 MUL                  ERn                   Rm  1111_nnn0_mmmm_0100                      F8C4
                 DIV                  ERn                   Rm  1111_nnn0_mmmm_1001                      F8A9
                 INC                 [EA]                       1111_1110_0010_1111                      FE2F
                 DEC                 [EA]                       1111_1110_0011_1111                      FE3F
                  RT                                            1111_1110_0001_1111                      FE1F
                 RTI                                            1111_1110_0000_1111                      FE0F
                 NOP                                            1111_1110_1000_1111                      FE8F
