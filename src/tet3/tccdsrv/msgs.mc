; /*
;  *	SCCS: @(#)msgs.mc	1.1 (00/09/05)
;  *
;  *	UniSoft Ltd., London, England
;  *
;  * Copyright (c) 2000 The Open Group
;  * All rights reserved.
;  *
;  * No part of this source code may be reproduced, stored in a retrieval
;  * system, or transmitted, in any form or by any means, electronic,
;  * mechanical, photocopying, recording or otherwise, except as stated
;  * in the end-user licence agreement, without the prior permission of
;  * the copyright owners.
;  * A copy of the end-user licence agreement is contained in the file
;  * Licence which accompanies this distribution.
;  * 
;  * Motif, OSF/1, UNIX and the "X" device are registered trademarks and
;  * IT DialTone and The Open Group are trademarks of The Open Group in
;  * the US and other countries.
;  *
;  * X/Open is a trademark of X/Open Company Limited in the UK and other
;  * countries.
;  *
;  */
; 
; /************************************************************************
; 
; SCCS:   	@(#)msgs.mc	1.1 00/09/05 TETware release 3.8
; NAME:		msgs.mc
; PRODUCT:	TETware
; AUTHOR:	Andrew Dingwall, UniSoft Ltd.
; DATE CREATED:	July 2000
; 
; DESCRIPTION:
; 	tccdsrv event log message prototype file
; 
; MODIFICATIONS:
; 
; ************************************************************************/


MessageIDTypedef=DWORD

MessageId=0x1
Severity=Informational
Facility=Application
SymbolicName=TET_INFO_MSG
Language=English
%1!s!
.

