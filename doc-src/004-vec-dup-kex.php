<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("The Vec and Dup functions and Xifrat1-Kex.I");

 $hdr1_1 = hc_H2("Restricted Commutativity of Vec and Dup over themselves");

 $algo_Vec = hc_Figure("The algorithm for the Vec function");
 $algo_Dup = hc_Figure("The algorithm for the Dup function");
 
 $hdr1_2 = hc_H2("The Xifrat1-Kex.I KEM");

 $algo_kem_keygen = hc_Figure("Xifrat1-Kex.I Key Generation");
 $algo_kem_enc = hc_Figure("Xifrat1-Kex.I Encapsulation");
 $algo_kem_dec = hc_Figure("Xifrat1-Kex.I Decapsulation");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  In this section, we present the Vec and the Dup function, discuss the
  properties needed for constructing key exchange from it, and present
  such construction.
</p>

<?= $hdr1_1 ?>

<p>
  The purpose of the Vec function is the same as that of the Blk function,
  except it works over a different domain. The Vec function works over 2
  cryptograms that's made up of 7 63-bit slices similar to Enc and Mlt.
  The cryptogram is also 448-bit long with 441 effective bits.
  The construction of Vec is structurally similar to Blk.
</p>

<p>
  Within the Vec function, each of the 63-bit slices are ''hashed'' in the
  Blk function, and applied sequentially twice interlaced with the other
  operand. An obvious flaw is that, if we can <em>individually</em>
  brutal-force the slices, then we can evaluate the key exchange maths,
  which is a fatal break. (This had been an oversight in the previous
  versions of this paper, which we fix now, by appending the Roman numeral
  ".I" to the name of the scheme.)
</p>

<p>
  This is why, another layer is needed, which we call Dup. The purpose of
  Dup is, yet again, the same as Blk as well as Vec, but this time,
  the 7 slices are ''hashed'', requiring attacker to brutal force
  &<$ 7 &times; 63 = 441 &> bits. While this is a overkill for almost every
  scenario, we leave this as an overhead in case any powerful cryptanalytic
  attack is discovered.
</p>

<figure class="algorithm">
  <figcaption><?= $algo_Vec ?></figcaption>
  <ul>
    <li>Input: &<$ A=(A_0 A_1 ... A_6) , B=(B_0 B_1 ... B_6) &></li>
    <li>Output: &<$ C=(C_0 C_1 ... C_6) &></li>
  </ul>
  <p>Steps:</p>
  <ul>
    <li>&<$ C_0 =
      (A_0 A_1 ... A_6) (B_0 B_1 ... B_6)
      (A_0 A_1 ... A_6) (B_0 B_1 ... B_6) &></li>
    <li>&<$ C_1 =
      (A_1 A_2 ... A_0) (B_1 B_2 ... B_0)
      (A_1 A_2 ... A_0) (B_1 B_2 ... B_0) &></li>
    <li>&<$ C_2 =
      (A_2 A_3 ... A_1) (B_2 B_3 ... B_1)
      (A_2 A_3 ... A_1) (B_2 B_3 ... B_1) &></li>
    <li> ... </li>
    <li>&<$ C_6 =
      (A_6 A_0 ... A_5) (B_6 B_0 ... B_5)
      (A_6 A_0 ... A_5) (B_6 B_0 ... B_5) &></li>
  </ul>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_Dup ?></figcaption>
  <ul>
    <li>Input: &<$ A=(A_0 A_1) , B=(B_0 B_1) &></li>
    <li>Output: &<$ C=(C_0 C_1) &></li>
  </ul>
  <p>Steps:</p>
  <ul>
    <li>&<$ C_0 =
      (A_0 A_1) (B_0 B_1)
      (A_0 A_1) (B_0 B_1) &></li>
    <li>&<$ C_1 =
      (A_1 A_0) (B_1 B_0)
      (A_1 A_0) (B_1 B_0) &></li>
  </ul>
</figure>

<p>
  For ease of readability, &<$ D(D(a,b),c) &>
  will be rewritten as &<$ (a &#x2219; b &#x2219; c) &> .
</p>

<?= $hdr1_2 ?>

<p>
  As we've had the generalized restricted-commutativity property, we can
  construct a 9-variable key agreement scheme by laying out them in a square
  like this:
</p>

<p> &<$ (abc)(def)(ghi) = (adg)(beh)(cfi) &> </p>

<p>
  For ease of visualized intuiation, we lay them graphically:
</p>

<pre>// 9 variables //
a b c
d e f
g h i
</pre>

<p>
  The sum is identical regardless whether the matrix is evaluated row-first
  or column-first. Thus we take the middle column as the public key and the
  middle row as the ciphertext; &<$ b, h &> as "server-side" private key,
  and &<$ d, f &> as "client-side" secret share; The variables
  &<$ a, c, e, g, i &> are used as public information. The public key and
  the ciphertext are both in some kind of "sandwich" structure we mentioned
  in the previous section, which we believe makes recovering private keys
  impossible.
</p>

<p>
  It is obvious at this point that the public information can be derived from
  a seed using some extendable output function (XOF),
  (prior art: <?= cite("ref-newhope") ?>). We instantiate such XOF with
  SHAKE-128. We take 448-bit in turn, interpret it as 7 64-bit
  unsigned integers in little-endian and clear each of their top bits, and
  generate 5 of these and assign them to &<$ a, c, e, g, i &> in order.
  We denote this XOF as &<$ \Hx_{[448-7]&times;2}(seed) &> .
</p>

<figure class="algorithm">
  <figcaption><?= $algo_kem_keygen ?></figcaption>
  <ol>
    <li> Uniformly randomly generate choose a &<$ seed &> , </li>
    <li> Generate &<$ a, c, e, g, i &> using
      &<$ \Hx_{[448-7]&times;2}(seed) &> , </li>
    <li> Uniformly randomly generate 2 cryptograms &<$ b, h &> , </li>
    <li> Compute &<$ p = (b &#x2219; e &#x2219; h) &> , </li>
    <li> Return &<$ pk = ( seed , p ) &> as the public key and
      &<$ sk = ( seed , b , h ) &> as the private key. </li>
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_kem_enc ?></figcaption>
  <ol>
    <li> Expand &<$ seed &> into &<$ a, c, e, g, i &>
      using &<$ \Hx_{[448-7]&times;2}(seed) &> , </li>
    <li> Uniformly randomly generate 2 cryptograms &<$ d, f &> , </li>
    <li> Compute &<$ ss =
      (a &#x2219; d &#x2219; g)
      &#x2219; pk &#x2219;
      (c &#x2219; f &#x2219; i) &> , </li>
    <li> Compute &<$ ct = (d &#x2219; e &#x2219; f) &> , </li>
    <li> Return &<$ ss &> as shared secret and &<$ ct &> as ciphertext . </li>
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_kem_dec ?></figcaption>
  <ol>
    <li>
      Return &<$ ss =
      (a &#x2219; b &#x2219; c)
      &#x2219; ct  &#x2219;
      (g &#x2219; h &#x2219; i) &> as shared secret.
    </li>
  </ol>
</figure>

<table class="infobox">
  <thead>
    <th colspan="2">Parameters</th>
  </thead>

  <tbody>
    <tr><th>private key bytes</th><td>232</td></tr>
    <tr><th>public key bytes</th><td>120</td></tr>
    <tr><th>ciphertext bytes</th><td>112</td></tr>
  </tbody>
</table>
