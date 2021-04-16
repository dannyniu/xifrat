<?php
 require_once(getenv("HARDCOPY_SRCINC_MAIN"));

 $hdr1 = hc_H1("Xifrat Elements - The \"Entropic\" Quasigroup");
 $hdr2 = hc_H2("How did we choose such entropoid");

 $algo1 = hc_Figure('Quasigroup Search Program');

 $hdr3 = hc_H2("Miscellaneous information about the quasigroup");

 if( !hcPageBegin() ) return;
?>

<?= $hdr1 ?>

<p>
  The core element of Xifrat cryptosystem is a entropic quasigroup of
  8 elements. We represent the binary operation of this group as
  <span class="math">
    <?= mEval(
      "f : &integers;_8 &times; &integers;_8".
      "&rightarrow; &integers;_8") ?>
  </span>.
  This groupoid has the following properties:
</p>

<ul>
  <li>Non-Associative <em>In General</em>:
    that is, for most cases,
    <span class="math">
      <?= mEval("f(f(a,b),c) &ne; f(a,f(b,c))") ?>
    </span>
  </li>

  <li>Non-Commutative <em>In General</em>:
    that is, for most cases,
    <span class="math">
      <?= mEval("f(a,b) &ne; f(b,a)") ?>
    </span>
  </li>

  <li>Restricted-Commutativity:
    that is, for all cases,
    <span class="math">
      <?= mEval("f(f(a,b),f(c,d)) = f(f(a,c),f(b,d))") ?>
    </span>
  </li>
</ul>

<p>
  The table for the quasigroup is as follow:
</p>

<pre>
  5   3   1   6   7   2   0   4
  3   5   0   2   4   6   1   7
  6   2   4   5   0   3   7   1
  4   7   6   1   3   0   2   5
  0   1   3   7   6   4   5   2
  7   4   2   0   5   1   6   3
  2   6   7   3   1   5   4   0
  1   0   5   4   2   7   3   6
</pre>

<p>
  The function <span class="math"><?= mEval("f(a,b)") ?></span> returns
  the value of the cell at <span class="math"><?= mEval("a") ?></span>'th
  row and <span class="math"><?= mEval("b") ?></span>'th column
  (indices are 0-based).
</p>

<?= $hdr2 ?>

<p>
  The first part is "why 8"? Actually, we originally had one of 16 elements;
  although it had all the desired property, its choice cannot be verified.
  So we went for one that can be verified as random. When we run our program
  to find a verifiably random quasigroup with 16 elements, it took so much
  time that we concluded that it's beyond the patience of any potential
  third-party verifiers, so we went for 8.
</p>

<p>
  Second, we have a list of desired property of the quasigroup. In addition to
  the arithmetic ones listed above, we have the following:
</p>

<ul>
  <li>The quasigroup table should not have obvious symmetry;</li>
  <li>The quasigroup table should not have any fixed points;</li>
</ul>

<p>
  With these requirements in mind, we created a simple C program that
  searched for candidate quasigroup tables.
  The program ran the following steps:
</p>


<figure class="algorithm">
  <figcaption><?= $algo1 ?></figcaption>
  <ol>
    <li>Create an array of shuffles as follow:
      <ol>
        <li>Seed an instance of SHAKE-256 XOF
          <?= hcNamedHref("link-fips-202") ?>
          function with the NUL-terminated ASCII string:
          "<code>xifrat - public-key cryptosystem</code>"
        </li>

        <li>Create a list <span class="math"><?= mEval("U") ?></span>
          of shuffles one by one using the following steps:
          <ol>
            <li><code>[label-1]</code>: Read 8 octets from the XOF stream and
              interpret it as a 64-bit little-endian unsigned integer.
            </li>
            <li>If the number is greater than
              <span class="math">
                <?= mEval(
                  '\floor( (2^{64}-1) / 8! ) &middot; ( 8! )') ?>
              </span>
              then go to <code>[label-1]</code> and proceed from there again.
            </li>
            <li>Take the number modulo
              <span class="math"><?= mEval('8!') ?></span>
              and label it as (i.e., set it to)
              <span class="math"><?= mEval('s') ?></span>.
            </li>

            <li>Initialize the list <span class="math"><var>V</var></span>
              to be shuffled as [0,1,2 ... 7].</li>
            <li>For <span class="math"><var>i</var></span> in 8...2:
              <ol>
                <li><span class="math">
                  <?= mEval("p &leftarrow; s \modulo i") ?>
                </span></li>
                <li><span class="math">
                  <?= mEval("j &leftarrow; 8 - i") ?>
                </span></li>
                <li>Swap
                  <span class="math"><?= mEval("V_{p+j}") ?></span> and
                  <span class="math"><?= mEval("V_{j}") ?></span>.
                </li>
                <li><span class="math">
                  <?= mEval('s &leftarrow; \floor(s / i)') ?>
                </span></li>
              </ol>
            </li>
            <li>Append <span class="math"><var>V</var></span>
              to <span class="math"><var>U</var></span>
            </li>

          </ol>
        </li>
      </ol>
    </li>

    <li>Walk diagonally from top-right to bottom-right starting
      from the top-left corner to the bottom-right corner;
      the cells traversed in such pattern are labelled 0...63, and
      for each cell walked suchly:
      <ol>
        <li><code>[label-2]</code>: Set the value of the cell to
          that of the lowest index in the shuffle such that
          the constraints as set out and implied in the requirements
          are not violated.
        </li>
        <li>Recursively set the next cell similarly:
          <ol>
            <li>If at some point constraints are violated,
              try the next index in the shuffle; and if all indices
              are tried out, recursively fix it by trying the next index
              in the previous cells by reverting to <code>[label-2]</code>
              . Until:
            </li>
            <li>
              When all cells are set and no constraint is violated,
              output the table and return [SUCCESS].
            </li>
          </ol>
        </li>
      </ol>
    </li>
  </ol>
</figure>

<p>
  The source code for the program can be found at our GitHub repository:
  <?= hcURL("https://github.com/dannyniu/xifrat") ?>.
</p>

<?= $hdr3 ?>

<p>
  As we choose our quasigroup operation table in a verifiably random fashion,
  we should indicate how random the choice was. Below is
  "indices of freedom" of our quasigroup, laid in walking order from
  left to right and top to bottom. The smaller the index indicates the more
  free the choice of the cell value is; a value of 9 indicates that
  the constraints from other cells had determined the value of that cell
</p>

<pre>
  1   2   1   6   4   7   3   4
  7   2   3   1   1   3   5   9
  9   9   9   9   9   9   9   9
  9   9   9   9   9   9   9   9
  9   9   9   9   9   9   9   9
  9   9   9   9   9   9   9   9
  9   9   9   9   9   9   9   9
  9   9   9   9   9   9   9   9
</pre>

<p>
  Our program was originally written as a single-thread program. When
  it's found that more efficiency was more desired, we parallelized it
  with the <code>fork(2)</code> POSIX function, so that global states
  can still be shared by every function in the code, and minimal
  modification is needed.
</p>
