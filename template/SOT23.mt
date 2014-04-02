#version 1
#brief SOT23 and similar
#note Supports 3, 5, 6 and 8-pin parts, but not 4-pin. Also suitable for SOT353 series and others.
#pins 3 5 6 8
#param 3 @PT 0.95 @PP 2.3 @SH 1.0 @PW 1.1 @PL 0.8 @BW 2.9 @BL 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR SOT23
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw SOT
Op 0 0 0
At SMD
T0 0 {BL 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
DS {BW 2 / ~} {BL 2 / ~} {BW 2 /} {BL 2 / ~} {BP} 21
DS {BW 2 /} {BL 2 / ~} {BW 2 /} {BL 2 /} {BP} 21
DS {BW 2 /} {BL 2 /} {BW 2 / ~} {BL 2 /} {BP} 21
DS {BW 2 / ~} {BL 2 /} {BW 2 / ~} {BL 2 / ~} {BP} 21
DC {BW 4 / 0.15 - @RAD RAD BW 2 / - 0.15 +} {RAD BL 2 / - 0.2 +} {0.15 BW 2 / -} {0.2 BL 2 / - 0.1 +} {BP} 21
$PAD
Sh "{PN}" R {PL} {PW} 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
{? PT 3 = PN 3 < &}Po {SH 2 / ~} {PN 1.5 - 2 * PP *}
{? PT 3 = PN 3 = &}Po {SH 2 /} 0
{? PT 3 > PN PT 2 / ceil 1 + < &}Po {SH 2 / ~} {PT 2 / ceil 1 - 2 / PP * ~ PN 1 - PP * +}
{? PT 5 = PN 3 > &}Po {SH 2 /} {4.5 PN - 2 * PP *}
{? PT 5 > PN PT 2 / > &}Po {SH 2 /} {PT 0.75 * 0.5 + PN - PP *}
$EndPAD
$EndMODULE {NAME}
