<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("References");

 $refs = [];
 $refs["Bruck44"] = hcNamedAnchor("[Bruck44]", "ref-bruck44");
 $refs["Murdoch39"] = hcNamedAnchor("[Murdoch39]", "ref-murdoch39");
 $refs["Toyoda41"] = hcNamedAnchor("[Toyoda41]", "ref-toyoda41");
 $refs["Disguise"] = hcNamedAnchor("[Panny21]", "ref-2021-583");
 $refs["Xifrat0"] = hcNamedAnchor("[NN21]", "ref-xifrat0");
 $refs["Xifrat-"] = hcNamedAnchor("[Niu21]", "ref-xifrat-");
 $refs["Xifrat1"] = hcNamedAnchor("[Niu22]", "ref-xifrat1");

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
    <?= $refs["Bruck44"] ?>
    Richard H. Bruck;
    <i>Some Results in the Theory of Quasigroups</i>;
    In: <i>Transactions of the American Mathematical Society</i>
    55.1 (1944), pp. 19-52.
  </li>

  <li>
    <?= $refs["Murdoch39"] ?>
    David C. Murdoch;
    <i>Quasi-Groups Which Satisfy Certain Generalized
      Associative Laws</i>;
    In: <i>American Journal of Mathematics</i>
    61.2 (1939), pp.509-522.
  </li>

  <li>
    <?= $refs["Toyoda41"] ?>
    Koshichi Toyoda;
    <i>On axioms of linear functions</i>;
    In: <i>Proceedings of the Imperial Academy</i>
    17.7 (1941), pp.221-227.
  </li>
  
  <li>
    <?= $refs["Disguise"] ?>
    Lorenz Panny; 2021-05
    <i>Entropoids: Groups in Disguise</i>;
    <?= hcURL("https://ia.cr/2021/583") ?>
  </li>

  <li>
    <?= $refs["Xifrat0"] ?>
    Daniel Nager, and Jianfang "Danny" Niu; 2021-04
    <i>Xifrat - Compact Public-Key Cryptosystems based on Quasigroups</i>;
    <?= hcURL("https://ia.cr/2021/444") ?>
  </li>

  <li>
    <?= $refs["Xifrat-"] ?>
    Jianfang "Danny" Niu; 2021-04
    <i>Xifrat Cryptanalysis - Compute the Mixing Function Without the Key</i>;
    <?= hcURL("https://ia.cr/2021/487") ?>
  </li>

  <li>
    <?= $refs["Xifrat1"] ?>
    Jianfang "Danny" Niu; 2022-04
    <i>Resurrecting Xifrat - Compact Cryptosystems 2nd Attempt</i>;
    <?= hcURL("https://ia.cr/2022/429") ?>
  </li>
</ul>
