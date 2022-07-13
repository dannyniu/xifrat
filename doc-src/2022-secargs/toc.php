<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $Cover = "abs-cover";
 $Title = "Some Security Arguments For Xifrat1";

 hcAddPages("001-introduction");
 hcAddPages("002-qg-funcs");
 hcAddPages("003-oblivious-eval");
 hcAddPages("004-gt-attack");

 hc_StartAnnexes();
 hcAddPages("0a-references");

 hcFinish();
