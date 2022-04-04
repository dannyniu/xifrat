<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $Cover = "abs-cover";
 $Title = "Resurrecting Xifrat - Compact Cryptosystems 2nd Attempt";

 hcAddPages("001-background");
 hcAddPages("002-qg-blk");
 hcAddPages("003-enc-mlt-dss");
 hcAddPages("004-vec-kex");

 hc_StartAnnexes();
 hcAddPages("0a-references");

 hcFinish();
