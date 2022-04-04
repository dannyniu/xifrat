<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("The Enc and Mlt functions and Xifrat1-Sign");

 $hdr1_1 = hc_H2("Restricted Commutativity of Mlt over Enc (and vice versa)");

 $algo_Enc = hc_Figure("The algorithm for the Enc function");
 $algo_Mlt = hc_Figure("The algorithm for the Mlt function");

 $hdr1_2 = hc_H2("The Xifrat1-Sign DSS");

 $algo_dss_keygen = hc_Figure("Xifrat1-Sign Key Generation");
 $algo_dss_sign   = hc_Figure("Xifrat1-Sign Signature Generation");
 $algo_dss_verify = hc_Figure("Xifrat1-Sign Signature Verification");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  In this section, we present the Enc and Mlt functions, argue the security
  property for Enc empirically, and present construction for
  a digital signature scheme
</p>

<?= $hdr1_1 ?>

<p>
  The Enc function is designed to hide its right-hand operand, that is
  given &<$ P &> and &<$ C = \Enc(P,K) &> , it should be cryptographically
  impossible to discover &<$ K &> (assuming &<$ K &> is randomly generated).
  The construction of Enc is inspired by the "sandwich" structure of some
  of the tweakable blockcipher modes such as XEX (xor-encrypt-xor) and
  EME (encrypt-mask-encrypt). The way we design the Enc function make it
  impossible to make it restricted-commutative with itself, so we introduced
  a second Mlt function, which together with Enc have the following
  property:
</p>

<p> &<$ M( E(a,b), E(c,d) ) = E( M(a,c), M(b,d) ) &> </p>

<p>
  As a conservative design decision, we choose to have a 7-slice
  Feistel-network-like structure for cryptograms for Enc and Mlt, totalling
  448 bits (441 effective), aiming at offering more than 192-bit security.
</p>

<figure class="algorithm">
  <figcaption><?= $algo_Enc ?></figcaption>
  <ul>
    <li>Input: &<$ A=(A_0 A_1 ... A_6) , B=(B_0 B_1 ... B_6) &></li>
    <li>Output: &<$ C=(C_0 C_1 ... C_6) &></li>
  </ul>
  <p>Steps:</p>
  <ul>
    <li>&<$ C_0 = (...((B_0 A_0 B_0) B_1 A_1 B_1) ... ) B_6 A_6 B_6 &></li>
    <li>&<$ C_1 = (...((B_1 A_1 B_1) B_2 A_2 B_2) ... ) B_0 A_0 B_0 &></li>
    <li>&<$ C_2 = (...((B_2 A_2 B_2) B_3 A_3 B_3) ... ) B_1 A_1 B_1 &></li>
    <li> ... </li>
    <li>&<$ C_6 = (...((B_6 A_6 B_6) B_0 A_0 B_0) ... ) B_5 A_5 B_5 &></li>
  </ul>
</figure>

<p>
  We put parentheses in, but it's just equivalent to the actual operation
  where all operands are just evaluated in order.
</p>

<figure class="algorithm">
  <figcaption><?= $algo_Mlt ?></figcaption>
  <ul>
    <li>Input: &<$ A=(A_0 A_1 ... A_6) , B=(B_0 B_1 ... B_6) &></li>
    <li>Output: &<$ C=(C_0 C_1 ... C_6) &></li>
  </ul>
  <p>Steps:</p>
  <ul>
    <li>&<$ C_i = A_i B_i , i &in; &lbrace; 0, 1, 2, 3, 4, 5, 6 &rbrace; &></li>
  </ul>
</figure>

<?= $hdr1_2 ?>

<p>
  Now we present the Xifrat1-Sign digital signature scheme. The general
  structure is similar to Xifrat0-Sign, but use Enc and Mlt functions
  to actually achieve unforgeability.
</p>

<p>
  As with Xifrat0-Sign, we use a hash function, which is instantiated with
  the XOF SHAKE-256. We take its initial 448-bit output, interpret it as
  7 64-bit unsigned integers in little-endian, and clear each of their
  top bits. We denote this hash function as &<$ \Hx_{448-7}(m) &> .
</p>

<figure class="algorithm">
  <figcaption><?= $algo_dss_keygen ?></figcaption>
  <ol>
    <li>
      Uniformly randomly generate 3 cryptograms:
      &<$ C, K, &> and &<$ Q &> ,
    </li>

    <li>
      Compute &<$ P_1 = E(C,K) , P_2 = M(K,Q) &> ,
    </li>

    <li>
      Return public-key &<$ pk = ( C , P_1 , P_2 ) &>
      and private-key &<$ sk = ( C , K , Q ) &> .
    </li>
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_dss_sign ?></figcaption>
  <ol>
    <li> <b>Input:</b> &<$ m &> - the message </li>
    <li> Compute &<$ H = \Hx_{448-7}(m) &> , </li>
    <li> Compute &<$ S = E(H,Q) &> , </li>
    <li> Return &<$ S &> , </li>
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo_dss_verify ?></figcaption>
  <ol>
    <li> <b>Input:</b> &<$ m &> - the message , &<$ S &> - the signature </li>
    <li> Compute &<$ H = \Hx_{448-7}(m) &> , </li>
    <li> Compute &<$ T_1 = M( P_1 , S ) &> , </li>
    <li> Compute &<$ T_2 = E( M(C,H) , P_2 ) &> , </li>
    <li> If &<$ T_1 = T_2 &> return [VALID] ; otherwise return [INVALID].</li>
  </ol>
</figure>
