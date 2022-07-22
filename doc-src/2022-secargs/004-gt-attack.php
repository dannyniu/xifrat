<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Attack 2: Group Theoretic Analysis");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  This section discusses group theoretic cryptanalysis on Xifrat1. I had
  briefly discussed this with the author of <?= cite("ref-2021-583") ?> -
  we hold different opinion over this.
</p>

<p>
  As we said in <?= cite("ref-xifrat1") ?>, the 16x16 latin square was chosen
  randomly; and we assume its quasigroup operation is also random, in the sense
  that it <em>behave as if randomly</em>.
</p>

<p>
  It's a fact
  <?= cite("ref-bruck44")." ".
      cite("ref-murdoch39")." ".
      cite("ref-toyoda41") ?>
  that, any quasigroup, like that we've been using, can be
  decomposed into a group with 2 automorphisms:
</p>

<p>
  &<$ f(a,b) = g(a) + h(b) + c &> where &<$ f &> is the quasigroup operation,
  &<$ g &> and &<$ h &> are 2 automorphisms which we assume <em>are
  independent of each other</em> and <em>behave as if randomly</em>, and
  &<$ c &> is a constant from the quasigroup set, and
  &<$ + &> is the operation of the decomposed group.
</p>

<p>
  Now let's see an example:
</p>

<p style="line-height: 1.8;">
  &<$ g(x) + h(y) = u &>
  <br/>
  &<$ g(y) + h(x) = v &>
</p>

<p>
  If &<$ g &> and &<$ h &> are truely random, then the only way to find
  &<$ x , y &> from &<$ u , v &> would be to try every possible solution
  and verify each of them to find out. Because we generated our 16x16
  quasigroup randomly, we assume that the underlaying automorphisms
  fulfills this property. It is further assumed, that composition of
  randomly-behaving maps are also randomly-behaving.
</p>

<p>
  The Blk function can now be reverted, by first searching 16 independent
  quartets from the alternating phase, then solving the "cycling" equations
  system, which consists of 16 group equations, each with 16 automorphisms
  that we had assumed to be "randomly-behaving".
</p>

<p>
  There are 2 crucial assumptions we make, as the basis for believing that
  the mixing funcitons at higher layer are more difficult to to invert than
  the Blk function.
</p>

<ol>
  <li>
    The group automorphisms exploitation is the most efficient attack
    applicable to the cycling-alternating mixing function formula <em>when
    assuming</em> that there is a efficient way to evaluate the automorphisms.
  </li>

  <li>
    There is no efficient way to find or evaluate larger group automorphisms
    from the group and automorphisms underlaying the smaller abelian quasigroup
    <em>assuming</em> the underlaying abelian quasigroup is randomly-behaving.
  </li>
</ol>

<p>
  Additionally, we assume that the expansion result of abelian quasigroup
  formula at higher layer into group automorphisms is no more efficiently
  solvable than applying layer-by-layer approach according to the
  preceding list, as the expansion of the formula terms is polynomial (which
  we believe makes the solution of the equasions system super-exponential,
  but we have yet no way of being sure).
</p>
