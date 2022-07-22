<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Attack 1: Evaluate without Full Knowledge of 1 Operand");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  The first attack we discuss is the "evaluation without full knowledge of
  either operand" attack. This attack was present in a most fatal form in
  Xifrat0, and was quickly broken <?= cite("ref-xifrat-") ?> back then.
</p>

<p>
  The same apply to Xifrat1. Recall that the "Blk" function works over
  a vector of 16 quartets. If we can find either of &<$ u &> or &<$ v &> ,
  then we can use that knowledge to compute that function - because
  the alternating phase works parallelly over the vector of tritet.
  This attack has to be blocked at the cycling phase of a higher layer.
</p>

<p>
  The cycling phase at a higher layer mixes together the vector elements
  of the lower layer, making it necessary to recover the vector
  in its entirety to be able to compute the mixing function. This is why
  at 384 bits, we still need a "Dup" layer on top of the "Vec" layer.
</p>
