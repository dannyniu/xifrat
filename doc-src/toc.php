<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $Title = "Xifrat - Compact Public-Key Cryptosystems based on Quasigroups";
 $Cover = "abs-cover";

 hcAddPages("001-intro");
 hcAddPages("002-elements");
 hcAddPages("003-primitive");
 hcAddPages("004-schemes");

 hc_StartAnnexes();
 hcAddPages("0a-references");

 hcFinish();

