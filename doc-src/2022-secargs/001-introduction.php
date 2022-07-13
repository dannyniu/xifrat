<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Introduction");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  The 3rd round of NIST PQC project had recently completed and first set of
  candidates to be standardized is announced. Also announced is the NIST's
  intentions to standardize a more efficient general-purpose
  signature scheme. For the announced candidates -
</p>

<ul>
  <li>The most efficient and easiest to implement Dilithium has
    big cryptograms,</li>
  <li>The most compact Falcon has intricate implementation requirements,</li>
  <li>The one with the most confidence in security - SPHINCS+ has
    huge signatures.</li>
</ul>

<p>
  Xifrat1 <?= cite("ref-xifrat1") ?> is a new proposal based on
  abelian quasigroups, that provides compact cryptograms, and is reasonably
  efficient. The paper proposing Xifrat1 didn't touch deep on
  security arguments. This paper will provide some arguments for its security,
  and serve as future reference for reasoning and/or refuting its security.
</p>
