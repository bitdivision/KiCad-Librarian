#version 1
#brief Small Outline w/ Pins, Exposed pad
#note Suitable for SOIC and SSOP parts. With exposed pad.
#pins 9 11 ...
#param 9 @PT 1.27 @PP 5.6 @SH 1.75 @PW 0.65 @PL 2.2 @PWA 3.0 @PLA 3.6 @BW 5.0 @BL 0.2 @BP 0.65 @TS 0.1 @TP 20 @PSRA
$MODULE {NAME}
AR SOPX
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw SOIC SSOP TSSOP
Op 0 0 0
At SMD
T0 0 {BL 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
T2 {BW 2 / ~ BP 2 * -} {PT 1 - -0.25 * 0.5 + PP * PL - BP -} {BP 2 *} {BP 2 *} 0 {TP} N H 21 N "○"
DS {BW 2 / ~} {BL 2 / ~} {BW 2 /} {BL 2 / ~} {BP} 21
DS {BW 2 /} {BL 2 / ~} {BW 2 /} {BL 2 /} {BP} 21
DS {BW 2 /} {BL 2 /} {BW 2 / ~} {BL 2 /} {BP} 21
DS {BW 2 / ~} {BL 2 /} {BW 2 / ~} {BL 2 / ~} {BP} 21
{BW 6 / 0.1 - 0.3 min @RAD   0.25 @OFFS   RAD BW 2 / - OFFS + @XC   RAD BL 2 / - OFFS + @YC}
DC {XC} {YC} {XC RAD +} {YC} {BP} 21
{PT 1 - @PINS}
$PAD
{? PN PINS <=}Sh "{PN}" O {PW} {PL} 0 0 0
{? PN PINS >}Sh "{PN}" R {PWA} {PLA} 0 0 0
{? PN PINS >}.SolderPasteRatio {PSRA 2 /}
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
{? PN PINS 2 / <=}Po {SH 2 / ~} {PINS -0.25 * 0.5 - PN + PP *}
{? PN PINS 2 / > PN PINS <= &}Po {SH 2 /} {PINS 0.75 * 0.5 + PN - PP *}
{? PN PINS >}Po 0 0
$EndPAD
$EndMODULE {NAME}
