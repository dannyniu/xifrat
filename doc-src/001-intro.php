<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Introduction");
 $hdr2 = hc_H2("Parallel Efforts");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  Public-key cryptography is a branch of modern cryptography that studies
  the protection of secrecy and authenticity in an open and public environment.
  Two most important functionalities of public-key cryptography are
  digital signature (authenticity) and public-key encryption and (its functional
  equivalent) key exchange (secrecy).
</p>

<p>
  Two most prominent development occurred in the 1970s -
  the Diffie-Hellman key exchange <?= hcNamedHref("ref-dh-1976") ?>, and
  the RSA cryptosystem <?= hcNamedHref("ref-rsa-1978") ?>,
  with current cryptanalysis showing that at least 2048-bit parameters are
  required for 112-bit security level due to the
  "General Number Field Sieving" algorithm
  <?= hcNamedHref("ref-ll-gnfs-1993") ?>.
  The most prominent industry standard for RSA is
  <?= hcNamedHref("link-pkcs-1") ?>.
</p>

<p>
  The most compact public-key cryptosystems known today are based on
  the elliptic-curve discrete logarithm problem, with current cryptanalysis
  showing that only 256-bit parameters are needed for 128-bit security level
  due to Pollard's rho algorithm for discrete logarithm
  <?= hcNamedHref("ref-pollard-1978") ?>.
  The most prominent industry standard for elliptic-curve cryptography
  would be <?= hcNamedHref("link-sec-1") ?>.
</p>

<p>
  The search for compact public-key cryptosystem would have stopped when
  Berstein et al. introduced Curve25519 for key exchange and Ed25519 for
  digital signature
  <?= hcNamedHref("ref-25519-2005") ?>
  <?= hcNamedHref("ref-25519-2011") ?>,
  which offered the best-in-class efficiency, performance, and
  logical and practical security (that is, both mathematically correct and
  easy to implement correctly and free of side channels), if wasn't for
  there're polynomial-time <em>quantum computer</em> algorithms for
  integer factorization and discrete logarithm
  <?= hcNamedHref("ref-shor-1995") ?>.
</p>

<p>
  For the list of finalist and alternate candidates in
  the 3rd round of the NIST Post-Quantum Cryptography project
  <?= hcNamedHref("link-nist-pqc") ?>,
  ones with one compact cryptogram usually have another one that's very huge
  (compact public key for SPHINCS+ with huge signature, compact signature
  for Rainbow with huge public key), some with solid security records may
  have large cryptograms with no compact one at all (Classic McEliece).
  For the ones with overall acceptable cryptogram sizes (Dilithium, Falcon,
  Kyber, NTRU, Saber), those sizes are still larger than that of RSA by
  a factor of a single decimal digit, and larger than that of elliptic curve
  by an order of magnitude.
</p>

<p>
  In this paper, we propose 2 compact cryptosystems - 1 for key exchange and
  1 for digital signature - both based on a quasigroup with the
  special property of "restricted-commutativity". Although cryptosystems
  based on quasigroups had been proposed <?= hcNamedHref("ref-mqq") ?>
  and broken <?= hcNamedHref("ref-mqq-break") ?> before, our construction is
  completely different from theirs. While we cannot disprove the existence
  of efficient algorithm that break our cryptosystems, we present empirical
  arguments for the security of our cryptosystems. 
</p>

<p>
  The proposed cryptosystem along with the reference implementation
  are released to the public domain.
</p>

<?= $hdr2 ?>

<p>
  In an independent effort, D.Gligoroski had proposed
  <?= hcNamedHref("ref-entropic-quasigroup") ?>
  the use of quasigroups with "restricted-commutativity" in
  public-key cryptography and termed it "entropic" quasigroups - "entropoids".
</p>

<p>
  In his paper, key exchange and digital signature algorithms are built using
  a discrete-logarithm-like problem based on an underlaying
  mathematical structures that doesn't appear to admit cryptanalysis by
  Shor's algorithm. In an exchange taken between us, we acknowledged that
  1) his design have more solid foundation than ours;
  2) our design doesn't appear to provide adequate level of complexity
  to calm worries of cryptanalysis;
  3) our design appears to be more efficient than his.
</p>

<p>
  Finally, we'd like to note that, if his design withstands the test of time,
  our choice of verifiably random quasigroup can be beneficially used as
  a component in his schemes.
</p>
