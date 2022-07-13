<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("The Xifrat1 Construction");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  The cryptograms in Xifrat1 are vectors of 14 64-bit (63 effective) words,
  built from 3 layers, bootstrapped from a 8x8 randomly chosen
  abelian quasigroup whose set we call the "tritet". The cryptograms and
  each layer below it follow the "restricted-commutative" property.
</p>

<p style="line-height: 2;">
  &<$ P&lt;Q,n&gt; := ( Q^n , O : Q^n &times; Q^n &rarr; Q^n ) &>
  where <br/>

  &<$ O(a^n, b^n) = uvuv &>
  and<br/>

  &<$ a, b &in; Q &>
  <br/>
  &<$ u = (a_1 a_2 ... a_n | a_2 a_3 ... a_1 | ... | a_n a_1 ... a_{n-1} )&>
  <br/>
  &<$ v = (b_1 b_2 ... b_n | b_2 b_3 ... b_1 | ... | b_n b_1 ... b_{n-1} )&>
</p>

<small>
  We use 1-based indexing here for ease of notation.
  The Xifrat1 paper <?= cite("ref-xifrat1") ?> uses 0-based indexing
</small>

<p>
  The design of the new &<$ O &> mixing function has 2 phases - the
  cycling phase where &<$ u &> and &<$ v &> are computed from vector elements of
  &<$ a &> and &<$ b &>; the alternating phase of &<$ uvuv &> .

  By generalized restricted-commutativity, the cycling and alternating phases
  can be computed in either order and results in the same value output. The
  reslting template mixing function preserves the restricted-commutativity
  property from a lower layer to a higher layer

  The Xifrat1 cryptosystem uses 3 layers:
</p>

<p>
  The 1st layer is &<$ (B,Blk) := P&lt;{"\tritets"},21&gt; &>
</p>

<p>
  The 2nd layer is &<$ (V,Vec) := P&lt;B,7&gt; &>
</p>

<p>
  The final outter-mose layer is &<$ (D,Dup) := P&lt;V,2&gt; &>
</p>
