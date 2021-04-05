<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Xifrat Schemes");
 $hdr2 = hc_H2("Xifrat-Kex the Key Exchange");
 $algo1 = hc_Figure('The Xifrat-Kex Key Exchange Protocol');
 
 $hdr3 = hc_H2("Xifrat-Sign the Digital Signature Algorithm");
 $algo2 = hc_Figure('Xifrat-Sign Key Generation');
 $algo3 = hc_Figure('Xifrat-Sign Signature Generation');
 $algo4 = hc_Figure('Xifrat-Sign Signature Verification');

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  For readability purposes, we inherit at here, the notations
  used in the previous section.
</p>

<?= $hdr2 ?>

<p>
  Xifrat-Kex is a bi-party key exchange scheme similar to Diffie-Hellman
  <?= hcNamedHref("ref-dh-1976") ?>. Unlike recent lattice-based
  key encapsulation mechanisms or public-key encryption algorithms,
  Xifrat-Kex is "participant-symmetric" and has no "decryption failure".
</p>

<figure class="algorithm">
  <figcaption><?= $algo1 ?></figcaption>
  <ol>
    
    <li>
      Agree on a public cryptogram
      <span class="math"><var>C</var></span>
      with the peer.
    </li>

    <li>
      Generate a private key
      <span class="math"><?= mEval("K_{private}") ?></span>, and send
      <span class="math"><?= mEval("K_{self} = (CK_{private})") ?></span>
      to the peer.
    </li>

    <li>
      Receive the key share of the peer
      <span class="math"><?= mEval("K_{peer}") ?></span>, and compute
      <span class="math">
        <?= mEval("K_{shared} = K_{peer}(K_{private}C)") ?>
      </span>.
    </li>
    
  </ol>
</figure>

<p>
  <b>Correctness</b>: Suppose the private keys for oneself is K and
  for the peer is Q, the correctness of the key exchange is apparent from
  the restricted-commutativity property; the key exchange can be re-written as:
  <span class="math"><?= mEval("(CK)(QC) = (CQ)(KC) = K_{shared}") ?></span>
</p>

<?= $hdr3 ?>

<p>
  Xifrat-Sign is a digital signature scheme consisting of 3 algorithms for
  key-pair generation, signature generation, and verfication.
  Xifrat-Sign uses a hash function, which is instantiated with the XOF
  SHAKE-256 by taking its first 384 bits of output.
</p>

<figure class="algorithm">
  <figcaption><?= $algo2 ?></figcaption>
  <ol>

    <li>Generate 3 cryptograms:
      <span class="math"><var>C</var></span>,
      <span class="math"><var>K</var></span>, and
      <span class="math"><var>Q</var></span>.
    </li>

    <li>Compute
      <span class="math"><?= mEval("P_1 = (CK)") ?></span>,
      <span class="math"><?= mEval("P_2 = (KQ)") ?></span>,
    </li>

    <li>
      Return public-key
      <span class="math"><?= mEval("pk = (C, P_1, P_2)") ?></span>,
      and private-key
      <span class="math"><?= mEval("sk = (C, K, Q)") ?></span>,
    </li>
    
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo3 ?></figcaption>
  <ol>

    <li><b>Input:</b>
      <span class="math"><?= mEval("m") ?></span> - the message
    </li>

    <li>Compute
      <span class="math"><?= mEval('\{SHAKE256}_{384}(m)') ?></span> ,
      and zero-extend the output to the size of Xifrat cryptogram,
      and obtain the result as <span class="math"><var>H</var></span> .
    </li>

    <li>Compute
      <span class="math"><?= mEval("S = (HQ)") ?></span>
    </li>
    
    <li>Return <span class="math"><var>S</var></span></li>
    
  </ol>
</figure>

<figure class="algorithm">
  <figcaption><?= $algo4 ?></figcaption>
  <ol>

    <li><b>Input:</b>
      <span class="math"><?= mEval("m") ?></span> - the message ,
      <span class="math"><?= mEval("S") ?></span> - the signature
    </li>

    <li>Compute
      <span class="math"><?= mEval('\{SHAKE256}_{384}(m)') ?></span> ,
      and zero-extend the output to the size of Xifrat cryptogram,
      and obtain the result as <span class="math"><var>H</var></span> .
    </li>

    <li>Compute
      <span class="math">
        <?= mEval("T_1 = P_1 S") ?>
      </span>
    </li>

    <li>Compute
      <span class="math">
        <?= mEval("T_2 = (CH)P_2") ?>
      </span>
    </li>

    <li>
      If <span class="math"><?= mEval("T_1 = T_2")?></span>, 
      return [VALID] ; otherwise, return [INVALID] .
    </li>

  </ol>
</figure>

<p>
  <b>Correctness</b>: the formula for two verification transcripts:
  <span class="math">
    <?= mEval('T_1 = P_1 S = (CK)(HQ)') ?>
  </span>
  and
  <span class="math">
    <?= mEval('T_2 = (CH) P_2 = (CH)(KQ)') ?>
  </span> ,
  by restricted commutativity,
  <span class="math"><?= mEval("T_1 = T_2") ?></span>
</p>
