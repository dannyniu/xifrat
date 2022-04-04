<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Quasigroup and the Blk block function");

 $hdr1_1 = hc_H2("The restricted commutative quasigroup");
 $hdr1_2 = hc_H2("The generalized restricted commutativity");
 $hdr1_3 = hc_H2("The Blk block function");

 $algo1 = hc_Figure("The algorithm for the Blk function");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  In this section, we present the quasigroup table, discuss the property of
  restricted commutativity and its generalization (which we will be using
  in the PKC schemes), and present a construction that enlarges the
  quasigroup.
</p>

<?= $hdr1_1 ?>

<p>
  The quasigroup we're considering has the following properties:
</p>

<ul>
  <li>Non-Associative <i>In General</i>: that is,
    for most cases, &<$ (ab)c &ne; a(bc) &>
  </li>

  <li>
    Non-Commutative <i>In General</i>: that is,
    for most cases, &<$ ab &ne; ba &>
  </li>

  <li>
    Restricted-Commutativity: that is,
    for all cases, &<$ (ab)(cd) = (ac)(bd) &>
  </li>
</ul>

<p>
  Additionally, some properties are needed for basic security:
</p>

<ul>
  <li>The quasigroup table should overall be not symmetric;</li>
  <li>The quasigroup table should not have any fixed points;</li>
</ul>

<p>
  We observed that, in Xifrat0, as well as the
  <a href="https://crypto.stackexchange.com/q/88830/36960"
  >StackExchange post</a> that sparked all these discussion,
  the quasigroup tables had a regularity that, for each diagonal pair of
  equal table cells, the opposite diagonal is also equal. This appears to be
  a necessary but not sufficient condition for a power-of-2 table to
  be restricted-commutative; as for non-power-of-2 tables, experiment had
  shown this property does not apply to them.
</p>

<p>
  We used diagonal property for optimization and created a new program that
  searched for a random quasigroup table with the seed
  "<code>xifrat - public-key cryptosystem</code>" which is the same one
  that's used in Xifrat0. Although we had hoped for that our optimization
  can make it possible to find a 16-by-16 quasigroup, the poly-exponential-time
  complexity ultimately convinced us to give up.
</p>

<p>
  The source code for the new program
  can be found at our online git repository:
  <?= hcURL("https://github.com/dannyniu/xifrat") ?>
  The new table is as follow:
</p>

<pre>// Quasigroup generated using the new program //
  2   0   4   3   5   7   1   6
  1   5   3   4   0   6   2   7
  7   4   0   5   3   2   6   1
  0   2   7   6   1   4   5   3
  3   6   1   2   7   5   4   0
  6   3   5   0   4   1   7   2
  4   7   2   1   6   0   3   5
  5   1   6   7   2   3   0   4
</pre>

<p>
  The operation &<$ ab &> evaluates to the table cell at a'th row and b'th
  column, in 0-based index.
</p>

<p>
  We propose the <b>1st open problem</b> of this paper:
  Can we find a verifiably random 16-by-16 quasigroup table?
  Can we find one efficiently?
</p>

<?= $hdr1_2 ?>

<p>
  Now we introduce an important property, that is both useful, and comes
  naturally from restricted-commutativity:
  the generalized restricted commutativity.
</p>

<p>
  <b>Theorem 1.</b> <i>Left-associativity of distributiveness</i>
</p>

<p>
  That is: <br/>
  &<$ (a_1 b_1)(a_2 b_2) ... (a_n b_n) =
  (a_1 a_2 ... a_n)(b_1 b_2 ... b_n) &>
</p>

<p><b>Proof</b>:</p>

<p>
  Observe a case of 3 pairs: &<$ (ab)(cd)(ef) &> .<br/>
  due to restricted commutativity: &<$ (ac)(bd)(ef) &> , <br/>
  next, substitute &<$ g=(ac) , h=(bd) &> , we have: <br/>
  &<$ (gh)(ef) &> , <br/>
  again, due to restricted commutativity, we have: &<$ (ge)(hf) &> , <br/>
  substitute back, we have &<$ (ace)(bdf) &> , <br/>
  generalizing recursively, we have <b>Theorem 1.</b>
</p>

<p>
  <b>Property 1.</b> <i>Generalized Restricted-Commutativity</i>
</p>

<p>
  That is: <br/> &<$
  (x_{1,1} x_{1,2} ... x_{1,n})
  (x_{2,1} x_{2,2} ... x_{2,n})
  ...
  (x_{m,1} x_{m,2} ... x_{m,n})
  = &> <br/> &<$
  (x_{1,1} x_{2,1} ... x_{m,1})
  (x_{1,2} x_{2,2} ... x_{m,2})
  ...
  (x_{1,n} x_{2,n} ... x_{m,n})
  &>
</p>

<p><b>Proof</b>: From <b>Theorem 1.</b>, we have</p>

<p>
  &<$
  (x_{1,1} x_{1,2} ... x_{1,n})
  (x_{2,1} x_{2,2} ... x_{2,n})
  ...
  (x_{m,1} x_{m,2} ... x_{m,n})
  = &> <br/> &<$
  ((x_{1,1} x_{2,1})(x_{1,2} x_{2,2}) ... (x_{1,n} x_{2,n}))
  (x_{3,1} x_{3,2} ... x_{3,n}) ...
  (x_{m,1} x_{m,2} ... x_{m,n})
  = &> <br/> &<$
  ( ((x_{1,1} x_{2,1}) x_{3,1}) ((x_{1,2} x_{2,2}) x_{3,2}) ...
  (x_{1,n} x_{2,n} ... x_{m,n}))
  = &> <br/> &<$
  (x_{1,1} x_{2,1} ... x_{m,1})
  (x_{1,2} x_{2,2} ... x_{m,2})
  ...
  (x_{1,n} x_{2,n} ... x_{m,n})
  &>
</p>

<?= $hdr1_3 ?>

<p>
  The Blk block function is defined to enlarge the quasigroup - it operates
  on vector of 21 tritet bitstrings. This is 63-bit in total, which we fit
  in least-significant- bit&amp;byte -first order.
</p>

<figure class="algorithm">
  <figcaption><?= $algo1 ?></figcaption>
  <ul>
    <li>Input: &<$ A=(a_0 a_1 ... a_{20}) , B=(b_0 b_1 ... b_{20}) &></li>
    <li>Output: &<$ C=(c_0 c_1 ... c_{20}) &></li>
  </ul>
  <p>Steps:</p>
  <ul>
    <li>&<$ c_0 = (a_0 a_1 ... a_{20}) (b_0 b_1 ... b_{20}) &></li>
    <li>&<$ c_1 = (a_1 a_2 ... a_0) (b_1 b_2 ... b_0) &></li>
    <li>&<$ c_2 = (a_2 a_3 ... a_1) (b_2 b_3 ... b_1) &></li>
    <li>&<$ ... &></li>
    <li>&<$ c_{20} = (a_{20} a_0 ... a_{19}) (b_{20} b_0 ... b_{19}) &></li>
  </ul>
</figure>

<p>
  Programmatically, &<$ A, B, &> and &<$ C &> are represented as the
  <code>uint64_t</code> data type, with the top bit clear.
</p>

<p>
  <strong>There is one problem</strong> with the Blk function, that is,
  when it's given 2 vectors of repeated tritets, it produces a vector of
  repeated tritet. This is due to such input would produce same array of
  operations at every lane of output tritet. At the moment, we do not know
  if this is exploitable in the actual KEM and DSS scheme.
</p>

<p>
  We propose the <b>2nd open problem</b> of this paper:
  Is there other more efficient construction which is similar to both
  Blk and Enc, in that such construction provides key-hiding and
  is restricted-commutative over itself?
</p>
