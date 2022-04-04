<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Background");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  In this paper, we propose 2 group-theoretic compact public-key cryptosystems -
  a key ecapsulation mechanism (KEM) and a digital signature scheme (DSS).
  These cryptosystems are aimed at achieving at least 192-bit post-quantum
  security.
</p>

<p>
  Previously, most practical compact cryptosystems are based on elliptic curves,
  whether it's the pre-quantum ones based on EC discrete logarithm
  <?= cite("ref-sec1") ?>, or elliptic curve isogenies. The former are
  vulnerable to Shor's algorithm on quantum computers; while the latter
  are suffering from some performance problems. For example, SIKE
  <?= cite("ref-sike") ?> is a NIST 3rd-round KEM/PKE alternate candidate that
  executes in time that's an order of magnitude longer than lattice-based
  ones such as Kyber <?= cite("ref-kyber") ?> and Saber
  <?= cite("ref-saber") ?> , although, such difference is not too noticable;
  likewise, the later design that didn't manage to
  get into the NIST PQC project - SQISign <?= cite("ref-sqisign") ?>
  from Oct 2020 has signing time that's over 2 seconds long on
  modern workstation PCs.
</p>

<p>
  Group-theoretic cryptography are, in the opinion of the author, still in
  its infancy - with closures of various theoretical structures and properties
  being proposed and analyzed without anything remarkable turning up.
  Algebraic Eraser <?= cite("ref-algebraic-eraser") ?> being a prominent
  example based on braid group for key agreement that had failed to be
  standardized in an ISO/IEC standard; WalnutDSA <?= cite("ref-walnut-dsa") ?>
  being another prominent example that didn't pass the 1st round in the
  NIST PQC project. Both due to security issues.
</p>

<p>
  Xifrat aims to provide PQC schemes that're compact through use of a class of
  groupoid with the property of <em>restricted-commutativity</em>.
  Such groupoid was previously proposed in <?= cite("ref-xifrat0") ?>,
  however, a critical error was made in designing the "mixing" function,
  which resulted in a total break, just half a month after its publication.
  We retroactively name the scheme in that paper Xifrat0 (and Xifrat0-Kex and
  Xifrat0-Sign). In this paper, we revisit the design decisions, and devise
  new constructions that can be used securely (or more accurately:
  <em>may</em> be used securely <b>if</b> the underlying primitive can be
  proven secure).
</p>
