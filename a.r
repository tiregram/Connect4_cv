d = c(0.015527,0.050097,0.016601,0.045554,0.024527,0.050784,0.017888,0.048106,0.027905,0.017924,0.031265,0.04696,0.016392,0.048349,0.016458,0.044259,0.019201,0.03249,0.016516,0.044027,0.016423,0.046644,0.0167,0.039341,0.020963,0.044948,0.016486,0.046655,0.016319,0.029317,0.019141,0.048672,0.016531,0.047122,0.048529,0.016498,0.048556,0.016419,0.016577,0.047059,0.017055,0.047419,0.017813,0.045026,0.019203,0.025694,0.044535,0.046034,0.016233,0.047194,0.016089,0.038992,0.01652,0.042248,0.025717,0.031368,0.0165,0.016979,0.0192,0.045846,0.017136,0.049026,0.017123,0.047531,0.016678,0.039339,0.016595,0.036314,0.046544,0.016074,0.047612,0.017302,0.045205,0.016915,0.047589,0.018178,0.044798,0.030855,0.016537,0.016471,0.045348,0.016601,0.029323,0.045986,0.01678,0.047185,0.01628,0.047463,0.018034,0.019091,0.046635,0.048734,0.032028,0.017209,0.017136,0.049995,0.017865,0.046739,0.01674,0.048304,0.049459,0.022374,0.018522,0.047979,0.016926,0.016857,0.047691,0.016241,0.017372,0.021143,0.045739,0.016718,0.029399,0.046375,0.016682,0.032499,0.04747,0.044226,0.04722,0.01611,0.03966,0.029402,0.016605,0.046145,0.016466,0.036869,0.016762,0.045828,0.025149,0.038356,0.024357,0.043526,0.031925,0.016484,0.01676,0.045886,0.031471,0.01923,0.046635,0.031251,0.027687,0.046415,0.016773,0.046908,0.016725,0.046585,0.016755,0.046617,0.038146,0.016227,0.043967,0.018057,0.048948,0.01786,0.049072,0.016685,0.052115,0.016477,0.017266,0.047574,0.018141,0.045701,0.021022,0.01786,0.045947,0.033148,0.024652,0.046234,0.016508,0.017797,0.045868,0.016591,0.04759,0.016375,0.042853,0.040486,0.034087,0.017288,0.016491,0.017368,0.045214,0.046052,0.017101,0.030044,0.047732,0.046228,0.016458,0.045974,0.016815,0.048004,0.018098,0.046562,0.016814,0.045966,0.016941,0.029402,0.04457,0.031371,0.017327,0.049447,0.047069,0.022132,0.046494,0.016877,0.046835,0.047428,0.016888,0.049943,0.016615,0.048874,0.018665,0.046145,0.016418,0.047541,0.016539,0.046879,0.031727,0.016536,0.016825,0.016629,0.046594,0.016781,0.019222,0.016824,0.04616,0.016954,0.046836,0.017733,0.025966,0.042212,0.022293,0.020987,0.042494,0.030808,0.0442,0.016517,0.048863,0.018005,0.045965,0.016796,0.048254,0.016597,0.045327,0.019895,0.047975,0.021177,0.047223,0.036304,0.046068,0.016515,0.047065,0.016218,0.045218,0.016348,0.046536,0.017211,0.028717,0.042168,0.017469,0.038373,0.016794,0.016653,0.046951,0.044447,0.017307,0.046962,0.016785,0.021134,0.03304,0.048451,0.016151,0.047264,0.046832,0.016329,0.031782,0.016345,0.018728,0.016359,0.046884,0.016567,0.017701,0.017535,0.01651,0.017013,0.044801,0.036798,0.016234,0.029655,0.035548)
t1 = table(cut(d,breaks=seq(0.015,0.053,0.003)))
barplot(t1,ylab = "Quantity of Execution ",xlab = "Time of computation")