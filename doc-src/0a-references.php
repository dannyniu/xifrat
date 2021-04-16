<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("References");

 $refs = [];
 $refs["DH-1976"] = hcNamedAnchor("[DH76]", "ref-dh-1976");
 $refs["RSA-1978"] = hcNamedAnchor("[RSA78]", "ref-rsa-1978");
 $refs["LL-1993"] = hcNamedAnchor("[LL93]", "ref-ll-gnfs-1993");
 
 $refs["Po78"] = hcNamedAnchor("[Po78]", "ref-pollard-1978");
 $refs["Curve"] = hcNamedAnchor("[Curve25519]", "ref-25519-2005");
 $refs["EdDSA"] = hcNamedAnchor("[EdDSA]", "ref-25519-2011");
 $refs["Shor95"] = hcNamedAnchor("[Shor95]", "ref-shor-1995");

 $refs["MQQ"] = hcNamedAnchor("[GMK08]", "ref-mqq");
 $refs["MQQ-break"] = hcNamedAnchor("[FOPG10]", "ref-mqq-break");

 $refs["EQG"] = hcNamedAnchor("[Glig21]", "ref-entropic-quasigroup");
 
 $refs["FIPS-202"] = hcNamedAnchor("[FIPS-202]", "link-fips-202");
 $refs["NIST-PQC"] = hcNamedAnchor("[NIST-PQC]", "link-nist-pqc");
 $refs["PKCS#1"] = hcNamedAnchor("[PKCS#1]", "link-pkcs-1");
 $refs["SEC#1"] = hcNamedAnchor("[SEC#1]", "link-sec-1");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<style>
 ul#references li {
   margin:   2mm 0mm;
 }
</style>

<ul id="references">
  <li>
    <?= $refs["Curve"] ?> D.J.Bernstein,
    <i>Curve25519: new Diffie-Hellman speed records</i>
    <?= hcURL("https://cr.yp.to/papers.html#curve25519") ?>
  </li>

  <li>
    <?= $refs["DH-1976"] ?> W.Diffie, M.E.Hellman,
    <i>New Directions in Cryptography</i>.
    1976 IEEE Transactions on Information Theory;
    Volume: 22, Issue 6, pp 644-654;
    <?= hcURL("https://doi.org/10.1109/TIT.1976.1055638") ?>
  </li>

  <li>
    <?= $refs["EdDSA"] ?>
    D.J.Bernstein, N.Duif, T.Lange, P.Schwabe, B.Yang
    <i>High-speed high-security signatures</i>
    <?= hcURL("https://cr.yp.to/papers.html#ed25519") ?>
  </li>

  <li>
    <?= $refs["MQQ-break"] ?>
    J.Faug&egrave;re, R.S.&Oslash;deg&aring;rd, L.Perret, D.Gligoroski,
    <i>Analysis of the MQQ Public Key Cryptosystem</i>
    2021 Cryptology and Network Security;
    Volume: 6467, pp169-183;
    <?= hcURL("https://doi.org/10.1007/978-3-642-17619-7_13") ?>
  </li>

  <li>
    <?= $refs["MQQ"] ?>
    D.Gligoroski, S.Markovski, S.J.Knapskog,
    <i>Public Key Block Cipher Based on Multivariate Quadratic Quasigroups</i>
    <?= hcURL("https://eprint.iacr.org/2008/320") ?>
  </li>

  <li>
    <?= $refs["EQG"] ?>
    D.Gligoroski,
    <i>Entropoid Based Cryptography</i>
    <?= hcURL("https://eprint.iacr.org/2021/469") ?>
  </li>
  
  <li>
    <?= $refs["LL-1993"] ?> A.K.Lenstra, H.W.Lenstra,
    <i>The Development of the Number Field Sieve</i>.
    Lecture Notes in Mathematics, vol. 1554, Berlin, Springer-Verlag, 1993.
  </li>

  <li>
    <?= $refs["Po78"] ?> J.M.Pollard,
    <i>Monte Carlo methods for index computation (mod p)</i>.
    Mathematics of Computations;
    Volume: 32, Issue 143, pp 918-924;
    <?= hcURL("https://doi.org/10.1090/S0025-5718-1978-0491431-9") ?>
  </li>

  <li>
    <?= $refs["RSA-1978"] ?> R.L.Rivest, A.Shamir, L.Adleman,
    <i>A method for obtaining digital signatures and
      public-key cryptosystems</i>.
    1978 Communications of The ACM;
    Volume: 21, Issue: 2, pp 120-126;
    <?= hcURL("https://doi.org/10.1145/357980.358017") ?>
  </li>

  <li>
    <?= $refs["Shor95"] ?> P.Shor,
    <i>Polynomial-Time Algorithms for Prime Factorization and
      Discrete Logarithms on a Quantum Computer</i>.
    1995 arXiv: Quantum Physics;
    <?= hcURL("https://doi.org/10.1137/S0097539795293172") ?>
  </li>

  <li>
    <?= $refs["FIPS-202"] ?> NIST FIPS-202 <i>SHA-3 Standard:
    Permutation-Based Hash and Extendable-Output Functions</i> ;
    <?= hcURL("http://dx.doi.org/10.6028/NIST.FIPS.202") ?>
  </li>

  <li>
    <?= $refs["NIST-PQC"] ?>
    Post-Quantum Cryptography, Round 3 Submissions.
    <?= hcURL("https://csrc.nist.gov/Projects/".
              "post-quantum-cryptography/round-3-submissions") ?>
  </li>

  <li>
    <?= $refs["PKCS#1"] ?> K.M.Moriarty, B.Kaliski, J.Jonsson, A.Rusch,
    <i>PKCS #1: RSA Cryptography Standard Version 2.2</i>.
    <?= hcURL("https://tools.ietf.org/html/rfc8017") ?>
  </li>

  <li>
    <?= $refs["SEC#1"] ?>
    <i>SEC 1: Elliptic Curve Crptography</i>
    <?= hcURL("http://secg.org") ?>
  </li>
</ul>
