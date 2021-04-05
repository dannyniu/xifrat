<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Xifrat Primitive - The Mixing Function");
 $hdr2 = hc_H2("The Construction and Instantiation of the Mixing Function");

 $algo1 = hc_Figure(
   'Algorithm for <span class="math">'.
   mEval("m(r,k)").'</span>');

 $algo2 = hc_Figure(
   'Algorithm for <span class="math">'.
   mEval("generator(x)").'</span>');

 $hdr3 = hc_H2("The Arguments for Security");
 $hdr4 = hc_H2("Proof of Correctness");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  The next building block in Xifrat, is the mixing function
  <span class="math"><?= mEval("c = m(r,k)") ?></span>,
  where <span class="math"><?= mEval("c, r, k") ?></span> are "cryptograms"
  of Xifrat. The function has the following property:
</p>

<ul>
  <li>Restricted-Commutativity, as is the case with
    <span class="math"><?= mEval("f(a,b)") ?></span>
  </li>

  <li>Resistance to Key-Recovery: that is, given any number of pairs of
    <span class="math"><?= mEval("c") ?></span> and
    <span class="math"><?= mEval("r") ?></span>, it should be
    computationally infeasible to find
    <span class="math"><?= mEval("k") ?></span>.
  </li>

  <li>Resistance to Cryptogram Prediction: that is, given
    <span class="math"><?= mEval("c'") ?></span> or
    <span class="math"><?= mEval("r'") ?></span> (but not both), and
    any number of such pair of cryptogram under a particular
    <span class="math"><?= mEval("k") ?></span>,
    it should be computationally infeasible to find
    <span class="math"><?= mEval("c'") ?></span> from
    <span class="math"><?= mEval("r'") ?></span> and vice-versa.
  </li>
</ul>

<?= $hdr2 ?>

<p>
  The mixing function <span class="math"><?= mEval("m()") ?></span>
  runs with 1 parameter: <span class="math"><var>N</var></span> - number of
  scalar elements from <span class="math"><?= mEval("&integers;_8") ?></span>
  in the cryptogram vector. The scalar elements indexed
  <span class="math"><var>i</var></span> in a cryptogram
  <span class="math"><var>p</var></span> is
  <span class="math"><?= mEval("p_i") ?></span>.
</p>

<figure class="algorithm">
  <figcaption><?= $algo1 ?></figcaption>
  <ol>
    <li>For <span class="math"><var>i</var></span> in [0...N-1]: <br/>
      <span class="math">
        <?= mEval("r_i &leftarrow; f(r_i , k_i)") ?>
      </span>
      <small> // initial mixing of cryptograms
        <span class="math"><var>r</var></span> with
        <span class="math"><var>k</var></span>
      </small>
    </li>

    <li>Given
      <span class="math">
        <?= mEval('U &leftarrow; \list(\generator(N&middot;N+1))') ?>
      </span>,
      for <span class="math"><var>i</var></span> in [0...N&middot;N]: <br/>
      <span class="math">
        <?= mEval("r_i &leftarrow; f(r_{U_i} , r_{U_{i+1}})") ?>
      </span>
      <small> // mixing of
        <span class="math"><var>r</var></span> with itself in
        random sequence to block certain types of
        divide-and-conquer attack.
      </small>
    </li>

    <li>For <span class="math"><var>i</var></span> in [0...N-1]: <br/>
      <span class="math">
        <?= mEval("r_i &leftarrow; f(r_i , k_i)") ?>
      </span>
      <small> // final mixing of cryptograms
        <span class="math"><var>r</var></span> with
        <span class="math"><var>k</var></span>
      </small>
    </li>
  </ol>
</figure>

<p>
  In the above algorithm, the <span class="math">list()</span> function
  takes the outputs of an iterator and assembles them in to an ordered list.
  The function <span class="math">generator()</span> is defined as follow:
</p>

<figure class="algorithm">
  <figcaption><?= $algo2 ?></figcaption>
  <ol>
    <li>Create a list <span class="math">
      <?= mEval(
        'U &leftarrow; \list( 7*(3&middot;i + 5)^{17} + 11 \mod N '.
        '\for i \in [0...N-1] )') ?>
    </span></li>
    <li>For <span class="math"><var>i</var></span> in [0...x-1]:
      <ol>
        <li>yield <span class="math"><?= mEval("U_{i \mod N}") ?></span></li>

        <li>If <span class="math">
          <?= mEval("(N - i \mod N) = 1") ?> </span>:
          <ol>
            <li>Copy
              <span class="math"><var>U</var></span> to
              <span class="math"><var>V</var></span>.
            </li>

            <li>
              <span class="math">
                <?= mEval(
                  'U_j &leftarrow; V_{U_j} \for j \in [0...N]') ?>
              </span>
            </li>
          </ol>
        </li>

        <li><span class="math"><?= mEval("i = i+1") ?></span></li>
      </ol>
    </li>
    <li>Return [END].</li>
  </ol>
</figure>

<p>
  For ease of implementation, we choose a single parameter
  <span class="math"><var>N</var> = 131</span>, resulting in
  a cryptogram size of 393 bits (rounded up to 50 octets), targeting
  192-bit overall security. <span class="math"><var>N</var></span> is
  chosen to be a prime so that it's easy to build a random sequence
  generator that consumes few working memory.
</p>

<p>
  As a cryptanalysis challenge proposed for the purpose of helping
  understand the properties of Xifrat cryptosystem better, we propose
  a toy parameter set <span class="math"><var>N</var></span> of 23
  targeting 32-bit security.
</p>

<?= $hdr3 ?>

<p>
  To argue for the security of <span class="math"><var>m</var>()</span>,
  we observe that the middle loop of the mixing function is actually
  <span class="math"><var>N</var></span> rounds of application of
  Feistel network, with each <b>2 rounds</b> consisting of mixing of
  <span class="math"><var>N</var></span> tritets with each other
  permuted by the <span class="math"><var>f</var>()</span> function.
  Unlike regular Feistel network where operands are mixed sequentially,
  we randomized the order of mixing in order to deter potential
  'divide-and-conquer' attacks.
</p>

<p>
  Algebraically, each 8 values of the tritet needs to be represented as
  64 &times; 64 sparse matrix - two 8 &times; 8 ones nested together, as
  an inner one and an outer one that receives operands differently
  depending on whether the operand is applied on the left or right.
  In other words, the arithmetic value of the matrix depends on
  whether it's used as row or column index when looking up in the sbox.
</p>

<p>
  Next, each tritet output from the mixing function
  <span class="math"><var>m</var>()</span> is the product of appying
  <span class="math">2<var>N</var></span> tritets - the
  <span class="math"><var>r</var></span> and
  <span class="math"><var>k</var></span> , where
  <span class="math"><var>N</var></span> is 131.
  We <em>roughly</em> estimate that potential algebraic attacks would
  either have to operate at very high degree, or produce large set of
  linear relationships with too many derived linearized variables
  to be practical.
</p>

<?= $hdr4 ?>

<style>
 span.
</style>

<p>
  To prove the correctness of "restricted-commutativity" of the
  <span class="math"><var>m</var>()</span> function, we'll first need
  a few propositions.
</p>

<p>
  <b>Notation 1.</b> We simplify, both
  entry-wise application of scalar elements of cryptogram vectors,
  and direct application on tritet operands,
  <span class="math"><?= mEval('f(a,b)') ?></span> as
  <span class="math"><?= mEval('ab') ?></span> , and
  <span class="math"><?= mEval('f(f(...(a,b),c...))') ?></span> as
  <span class="math"><?= mEval('abc...') ?></span> ;

  <?php $wv = "'"; ?>
  for a function
  <span class="math"><var>e</var>()</span> defined later, we denote
  <span class="math"><?= mEval("e(a)") ?></span> as
  <span class="math"><?= mEval("a$wv") ?></span> .
</p>

<p>
  <b>Prop 1.</b> <em>Left-associativity of distributiveness</em>
</p>

<p>
  That is:
  <span class="math">
    <?= mEval(
      "(a_1 b_1)(a_2 b_2) ... (a_n b_n) = ".
      "(a_1 a_2 ... a_n)(b_1 b_2 ... b_n)") ?>
  </span>
</p>

<p>
  <b>Proof</b>: <br/>
  observe a case of 3 pairs:
  <span class="math">
    <?= mEval(
      "(ab)(cd)(ef)") ?>
  </span> .<br/>
  due to restricted commutativity:
  <span class="math">
    <?= mEval(
      "(ac)(bd)(ef)") ?>
  </span> ,<br/>
  next, substitute
  <span class="math">
    <?= mEval(
      "g=(ac) , h = (bd)") ?>
  </span> , we have:<br/>
  <span class="math">
    <?= mEval(
      "(gh)(ef)") ?>
  </span> ,<br/>
  again, due to restricted-commutativity, we have:
  <span class="math">
    <?= mEval(
      "(ge)(hf)") ?>
  </span> ,<br/> substitute back, we have:
  <span class="math">
    <?= mEval(
      "(ace)(bdf)") ?>
  </span> ,<br/> generalizing recursively, we have <b>Prop 1.</b>
</p>

<p>
  <b>Definition 1.</b>
  <em>
    <span class="math"><?= mEval("e(a)") ?></span>
    applies the middle loop (step 2) of the algorithm for
    <span class="math"><?= mEval("m(r,k)") ?></span> to
    <span class="math"><var>a</var></span> ; and per
    <b>Notation 1.</b> results in
    <span class="math"><?= mEval("a$wv") ?></span> .
  </em>
</p>

<p>
  <b>Prop 2.</b> <span class="math"><?= mEval("e(ab)=e(a)e(b)") ?></span>
</p>

<p>
  <b>Proof</b>:
  Due to the structure of the middle loop of
  <span class="math"><var>m</var>()</span>, this is apparent by applying
  <b>Prop 1.</b>
</p>

<p>
  <b>Prop 3.</b>
  <span class="math">
    <?= mEval(
      "(a_1 a_2 a_3)(b_1 b_2 b_3)(c_1 c_2 c_3) = ".
      "(a_1 b_1 c_1)(a_2 b_2 c_2)(a_3 b_3 c_3)") ?>
  </span>
</p>

<p>
  <b>Proof</b>:
  From the left side of the equation, by <b>Prop 1.</b>, we have
</p>

<p><span class="math">
  <?= mEval("(a_1 a_2 a_3)(b_1 b_2 b_3)(c_1 c_2 c_3) =") ?>
</span></p>
<p><span class="math">
  <?= mEval("( (a_1 b_1)(a_2 b_2)(a_3 b_3) ) (c_1 c_2 c_3) =") ?>
</span></p>
<p><span class="math">
  <?= mEval("((a_1 b_1)c_1) ((a_2 b_2)c_2) ((a_3 b_3)c_3) =") ?>
</span></p>
<p><span class="math">
  <?= mEval("(a_1 b_1 c_1)(a_2 b_2 c_2)(a_3 b_3 c_3)") ?>
</span></p>

<p>
  <b>Main Proposition</b>
  <span class="math"><?= mEval("(ab)(cd) = (ac)(bd)") ?></span>
</p>

<p>
  <b>Proof</b>:
  observe that the <span class="math"><var>m</var>(a,b)</span> function can be
  represented using <span class="math"><?= mEval("e(x)") ?></span> as:
  <span class="math"><?= mEval("e(ab)b") ?></span> , we first rewrite
  the left side of the equation as:
</p>

<p><span class="math">
  <?= mEval("(ab)(cd) = (e(ab)b) (e(cd)d) = ") ?>
</span></p>
<p><span class="math">
  <?= mEval("(e(a)e(b)b) (e(c)e(d)d) = ") ?>
</span></p>
<p><span class="math">
  <?= mEval("(a$wv b$wv b) (c$wv d$wv d) = ") ?>
</span></p>
<p><span class="math">
  <?= mEval("e(a$wv b$wv b) e(c$wv d$wv d) (c$wv d$wv d) =") ?>
</span></p>
<p><span class="math">
  <?= mEval("(a$wv$wv b$wv$wv b$wv) (c$wv$wv d$wv$wv d$wv) (c$wv d$wv d) =") ?>
</span></p>

<p>Likewise for the right side</p>

<p><span class="math">
  <?= mEval("(ac)(bd) =") ?>
</span></p>
<p><span class="math">
  <?= mEval("(a$wv$wv c$wv$wv c$wv) (b$wv$wv d$wv$wv d$wv) (b$wv d$wv d) =") ?>
</span></p>

<p>
  By <b>Prop 3.</b>, the two expressions are equal.
</p>
