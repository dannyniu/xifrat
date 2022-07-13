<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("The digital signature and key exchange schemes");

 $hdr1_1 = hc_H2("The Xifrat1-Sign.I DSS");

 $algo_dss_keygen = hc_Figure("Xifrat1-Sign.I Key Generation");
 $algo_dss_sign   = hc_Figure("Xifrat1-Sign.I Signature Generation");
 $algo_dss_verify = hc_Figure("Xifrat1-Sign.I Signature Verification");

 $hdr1_2 = hc_H2("The Xifrat1-Kex.I KEM");

 $algo_kem_keygen = hc_Figure("Xifrat1-Kex.I Key Generation");
 $algo_kem_enc = hc_Figure("Xifrat1-Kex.I Encapsulation");
 $algo_kem_dec = hc_Figure("Xifrat1-Kex.I Decapsulation");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<?= $hdr1_1 ?>

<p>
  In this section, we present the Xifrat1-Sign.I digital signature scheme.
  The general structure is similar to Xifrat0-Sign, but uses the Dup function
  to actually achieve unforgeability.
</p>

<p>
  As with Xifrat0-Sign, we use a hash function, which is instantiated with
  the XOF SHAKE-256. We take its initial 896-bit output, interpret it as
  14 64-bit unsigned integers in little-endian, and clear each of their
  top bits. We denote this hash function as &<$ \Hx_{896-14}(m) &> .
</p>

<figure class="algorithm">
  <figcaption><?= $algo_dss_keygen ?></figcaption>
  <ol>
    <li>
      Uniformly randomly generate 3 cryptograms:
      &<$ c, k, &> and &<$ q &> ,
    </li>

    <li>
      Compute &<$ p_1 = D(c,k) , p_2 = D(k,q) &> ,
    </li>

    <li>
      Return public-key &<$ pk = ( c , p_1 , p_2 ) &>
      and private-key &<$ sk = ( c , k , q ) &> .
    </li>
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_dss_sign ?></figcaption>
  <ol>
    <li> <b>Input:</b> &<$ m &> - the message </li>
    <li> Compute &<$ h = \Hx_{896-14}(m) &> , </li>
    <li> Compute &<$ s = D(h,q) &> , </li>
    <li> Return &<$ s &> , </li>
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_dss_verify ?></figcaption>
  <ol>
    <li> <b>Input:</b> &<$ m &> - the message , &<$ S &> - the signature </li>
    <li> Compute &<$ h = \Hx_{896-14}(m) &> , </li>
    <li> Compute &<$ t_1 = D( p_1 , s ) &> , </li>
    <li> Compute &<$ t_2 = D( D(c,h) , p_2 ) &> , </li>
    <li> If &<$ t_1 = t_2 &> return [VALID] ; otherwise return [INVALID].</li>
  </ol>
</figure>

<p>
  The proof of correctness of the scheme is as follow:
</p>

<p>
  &<$ t_1 = D( p_1 , s ) = D( D(c,k) , D(h,q) ) &> <br/>
  &<$ t_2 = D( D(c,h) , p_2 ) = D( D(c,h) , D(k,q) ) &> <br/>
  By restricted commutativity, we have &<$ t_1 = t_2 &> .
</p>

<table class="infobox">
  <thead>
    <th colspan="2">Parameters</th>
  </thead>

  <tbody>
    <tr><th>private key bytes</th><td>560</td></tr>
    <tr><th>public key bytes</th><td>336</td></tr>
    <tr><th>signature bytes</th><td>112</td></tr>
  </tbody>
</table>

<?= $hdr1_2 ?>

<p>
  In this section, we present construction for key exchange.
</p>

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
  SHAKE-128. We take 896-bit in turn, interpret it as 14 64-bit
  unsigned integers in little-endian and clear each of their top bits, and
  generate 5 of these and assign them to &<$ a, c, e, g, i &> in order.
  We denote this XOF as &<$ \Hx_{[896-14]&times;5}(seed) &> .
</p>

<figure class="algorithm">
  <figcaption><?= $algo_kem_keygen ?></figcaption>
  <ol>
    <li> Uniformly randomly generate choose a 8-octet &<$ seed &> , </li>
    <li> Generate &<$ a, c, e, g, i &> using
      &<$ \Hx_{[896-14]&times;5}(seed) &> , </li>
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
      using &<$ \Hx_{[896-14]&times;5}(seed) &> , </li>
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
