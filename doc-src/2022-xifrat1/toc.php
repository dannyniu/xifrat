<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $Cover = "abs-cover";
 $Title = "Resurrecting Xifrat - Compact Cryptosystems 2nd Attempt";

 hcAddPages("001-background");
 hcAddPages("002-qg-funcs");
 hcAddPages("003-dss-kem");

 hc_StartAnnexes();
 hcAddPages("0a-references");

 hcFinish();
