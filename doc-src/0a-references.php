<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("References");

 $refs = [];
 $refs["AlgEr"] = hcNamedAnchor("[AE]", "ref-algebraic-eraser");
 $refs["Kyber"] = hcNamedAnchor("[Kyber]", "ref-kyber");
 $refs["NewHope"] = hcNamedAnchor("[NewHope]", "ref-newhope");
 $refs["Saber"] = hcNamedAnchor("[Saber]", "ref-saber");
 $refs["SEC#1"] = hcNamedAnchor("[SEC#1]", "ref-sec1");
 $refs["SIKE"] = hcNamedAnchor("[SIKE]", "ref-sike");
 $refs["SQISign"] = hcNamedAnchor("[SQISign]", "ref-sqisign");
 $refs["WDSA"] = hcNamedAnchor("[WalnutDSA]", "ref-walnut-dsa");
 $refs["Xifrat0"] = hcNamedAnchor("[NN21]", "ref-xifrat0");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<style>
 ul#references li {
   margin:  2mm 1mm;
 }
</style>

<ul id="references">
  <li>
    <?= $refs["AlgEr"] ?>
    Derek Atkins, P. Gunnells; 2015;
    <i>Algebraic Eraser (TM) : A lightweight, efficient
      asymmetric key agreement protocol for use in
      no-power, low-power, and IoT devices</i>;
    <?= hcURL("http://csrc.nist.gov/groups/ST/lwc-workshop2015/papers/session8-atkins-paper.pdf") ?>
  </li>

  <li>
    <?= $refs["Kyber"] ?>
    Joppe W. Bos, L. Ducas, Eike Kiltz, Tancrède Lepoint,
    Vadim Lyubashevsky, J. Schanck, P. Schwabe, and D. Stehlé; 2017-06
    <i>CRYSTALS-Kyber: A CCA-Secure Module-Lattice-Based KEM</i>
    <?= hcURL("https://pq-crystals.org/kyber") ?> ,
    <?= hcURL("https://ia.cr/2017/634") ?>
  </li>

  <li>
    <?= $refs["NewHope"] ?>
    Erdem Alkim and Léo Ducas and Thomas Pöppelmann and Peter Schwabe; 2015-11
    <i>CRYSTALS-Kyber: A CCA-Secure Module-Lattice-Based KEM</i>
    <?= hcURL("https://newhopecrypto.org") ?> ,
    <?= hcURL("https://ia.cr/2015/1092") ?>
  </li>

  <li>
    <?= $refs["Saber"] ?>
    Jan-Pieter D’Anvers, Angshuman Karmakar,
    Sujoy Sinha Roy, and Frederik Vercauteren; 2017-06
    <i>Saber: Module-LWR based key exchange,
      CPA-secure encryption and CCA-secure KEM</i>
    <?= hcURL("https://ia.cr/2018/230") ?>
  </li>

  <li>
    <?= $refs["SEC#1"] ?>
    <i>SEC 1: Elliptic Curve Crptography</i>
    <?= hcURL("http://secg.org") ?>
  </li>
  
  <li>
    <?= $refs["SIKE"] ?>
    David Jao, et al.; 2017
    <i>SIKE – Supersingular Isogeny Key Encapsulation</i>
    <?= hcURL("http://sike.org") ?>
  </li>
  
  <li>
    <?= $refs["SQISign"] ?>
    Luca De Feo, David Kohel, Antonin Leroux,
    Christophe Petit and Benjamin Wesolowski; 2020-10
    <i>SQISign: compact post-quantum signatures
      from quaternions and isogenies</i>;
    <?= hcURL("https://ia.cr/2020/1240") ?>
  </li>
  
  <li>
    <?= $refs["WDSA"] ?>
    Iris Anshel, Derek Atkins, Dorian Goldfeld,
    and Paul E Gunnells; 2020-10
    <i>WalnutDSA(TM): A Quantum-Resistant Digital Signature Algorithm</i>;
    <?= hcURL("https://ia.cr/2017/058") ?>
  </li>
  
  <li>
    <?= $refs["Xifrat0"] ?>
    Daniel Nager, and Jianfang "Danny" Niu; 2020-10
    <i>Xifrat - Compact Public-Key Cryptosystems based on Quasigroups</i>;
    <?= hcURL("https://ia.cr/2021/444") ?>
  </li>
</ul>
