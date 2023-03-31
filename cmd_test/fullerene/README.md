# C60Xm isomer enumeration test

Enumeration C60Xm isomers with the assistance of [usenauty](https://github.com/Franklalalala/usenauty). The coordinates are in `C60_000001812opted.xyz` file.

[usenauty](https://github.com/Franklalalala/usenauty) is a lightweight tool to enumerate non-isomorphic addition patterns with [nauty](https://doi.org/10.1016/j.cpc.2020.107206) algorithm which is created by Brendan D. McKay. The original modification is performed in [usenauty](https://github.com/saltball/usenauty) by XJTU-ICP member Y. B. Han. This test is conducted with a branch version of it.

The test script could be found in [speed_test.py](https://github.com/Franklalalala/usenauty/cmd_test/fullerene),  see more test in [cmd_test](https://github.com/Franklalalala/usenauty/cmd_test).

| addon num        | 2      | 3      | 4       | 5         | 6          | 7           | 8             | 10             |
| ---------------- | ------ | ------ | ------- | --------- | ---------- | ----------- | ------------- | -------------- |
| permutation      | 1770   | 34220  | 487,635 | 5,461,512 | 50,063,860 | 386,206,920 | 2,558,620,845 | 75,394,027,566 |
| isomer           | 23     | 303    | 4,190   | 45,718    | 418,470    | 3,220,218   | 21,330,558    | 628,330,629    |
| time (s)         | 0.001  | 0.004  | 0.053   | 0.461     | 4.074      | 30.116      | 200.605       | 5917.19        |
| time to file (s) | 0.0688 | 0.0812 | 0.203   | 0.738     | 10.116     | 34.777      | 604.092       | xxx*           |

The `permutation` row is the combinatorial number of $\rm C_{60}^{m}$ for all possible  $\rm C_{60}X_m$ isomers.

The `isomer` row is the non-isomerphic patterns enumerated by [usenauty](https://github.com/Franklalalala/usenauty).

The `time` row is the enumeration time needed. (Conducted by a specific version, could be requested from the authors)

The `time to file` row is the execution time to enumerate and write to a file. (Output files for addon number 6, 7, 8 could be requested from the authors)

*This test is terminated due to the long execution time and the huge output file.
