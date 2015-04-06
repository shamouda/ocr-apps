#include "test.h"
 one_line_type asinhf_vec[] = {
{14, 0,123,__LINE__, 0xbff0416d, 0x20000000, 0xbff33333, 0x33333333},			/* -1.01597=f(-1.20000)*/
{14, 0,123,__LINE__, 0xbff02723, 0xa0000000, 0xbff30a3d, 0x70a3d70a},			/* -1.00955=f(-1.19000)*/
{14, 0,123,__LINE__, 0xbff00cb9, 0x00000000, 0xbff2e147, 0xae147ae1},			/* -1.00310=f(-1.18000)*/
{11, 0,123,__LINE__, 0xbfefe45a, 0x00000000, 0xbff2b851, 0xeb851eb8},			/* -0.99662=f(-1.17000)*/
{11, 0,123,__LINE__, 0xbfefaefe, 0xc0000000, 0xbff28f5c, 0x28f5c28f},			/* -0.99011=f(-1.16000)*/
{11, 0,123,__LINE__, 0xbfef795f, 0xa0000000, 0xbff26666, 0x66666666},			/* -0.98356=f(-1.15000)*/
{11, 0,123,__LINE__, 0xbfef437c, 0x80000000, 0xbff23d70, 0xa3d70a3d},			/* -0.97698=f(-1.14000)*/
{11, 0,123,__LINE__, 0xbfef0d54, 0xc0000000, 0xbff2147a, 0xe147ae14},			/* -0.97037=f(-1.13000)*/
{11, 0,123,__LINE__, 0xbfeed6e8, 0x00000000, 0xbff1eb85, 0x1eb851eb},			/* -0.96373=f(-1.12000)*/
{11, 0,123,__LINE__, 0xbfeea035, 0xe0000000, 0xbff1c28f, 0x5c28f5c2},			/* -0.95705=f(-1.11000)*/
{11, 0,123,__LINE__, 0xbfee693d, 0xe0000000, 0xbff19999, 0x99999999},			/* -0.95034=f(-1.10000)*/
{11, 0,123,__LINE__, 0xbfee31ff, 0xe0000000, 0xbff170a3, 0xd70a3d70},			/* -0.94360=f(-1.09000)*/
{11, 0,123,__LINE__, 0xbfedfa7b, 0x20000000, 0xbff147ae, 0x147ae147},			/* -0.93682=f(-1.08000)*/
{11, 0,123,__LINE__, 0xbfedc2af, 0x40000000, 0xbff11eb8, 0x51eb851e},			/* -0.93001=f(-1.07000)*/
{11, 0,123,__LINE__, 0xbfed8a9b, 0xe0000000, 0xbff0f5c2, 0x8f5c28f5},			/* -0.92317=f(-1.06000)*/
{11, 0,123,__LINE__, 0xbfed5240, 0xe0000000, 0xbff0cccc, 0xcccccccc},			/* -0.91629=f(-1.05000)*/
{11, 0,123,__LINE__, 0xbfed199d, 0xa0000000, 0xbff0a3d7, 0x0a3d70a3},			/* -0.90937=f(-1.04000)*/
{11, 0,123,__LINE__, 0xbfece0b1, 0xa0000000, 0xbff07ae1, 0x47ae147a},			/* -0.90242=f(-1.03000)*/
{11, 0,123,__LINE__, 0xbfeca77c, 0xc0000000, 0xbff051eb, 0x851eb851},			/* -0.89544=f(-1.02000)*/
{11, 0,123,__LINE__, 0xbfec6dfe, 0x60000000, 0xbff028f5, 0xc28f5c28},			/* -0.88842=f(-1.01000)*/
{14, 0,123,__LINE__, 0xbfec3436, 0x60000000, 0xbfefffff, 0xfffffffe},			/* -0.88137=f(-0.01000)*/
{13, 0,123,__LINE__, 0xbfebfa24, 0x20000000, 0xbfefae14, 0x7ae147ac},			/* -0.87428=f(-0.99000)*/
{13, 0,123,__LINE__, 0xbfebbfc7, 0x80000000, 0xbfef5c28, 0xf5c28f5a},			/* -0.86716=f(-0.98000)*/
{13, 0,123,__LINE__, 0xbfeb851f, 0xc0000000, 0xbfef0a3d, 0x70a3d708},			/* -0.86000=f(-0.97000)*/
{13, 0,123,__LINE__, 0xbfeb4a2c, 0xe0000000, 0xbfeeb851, 0xeb851eb6},			/* -0.85280=f(-0.96000)*/
{13, 0,123,__LINE__, 0xbfeb0eee, 0x60000000, 0xbfee6666, 0x66666664},			/* -0.84557=f(-0.95000)*/
{13, 0,123,__LINE__, 0xbfead364, 0x00000000, 0xbfee147a, 0xe147ae12},			/* -0.83830=f(-0.94000)*/
{13, 0,123,__LINE__, 0xbfea978d, 0x80000000, 0xbfedc28f, 0x5c28f5c0},			/* -0.83100=f(-0.93000)*/
{13, 0,123,__LINE__, 0xbfea5b6a, 0x40000000, 0xbfed70a3, 0xd70a3d6e},			/* -0.82365=f(-0.92000)*/
{13, 0,123,__LINE__, 0xbfea1efa, 0x40000000, 0xbfed1eb8, 0x51eb851c},			/* -0.81628=f(-0.91000)*/
{13, 0,123,__LINE__, 0xbfe9e23c, 0xe0000000, 0xbfeccccc, 0xccccccca},			/* -0.80886=f(-0.90000)*/
{13, 0,123,__LINE__, 0xbfe9a532, 0x20000000, 0xbfec7ae1, 0x47ae1478},			/* -0.80141=f(-0.89000)*/
{13, 0,123,__LINE__, 0xbfe967d9, 0x80000000, 0xbfec28f5, 0xc28f5c26},			/* -0.79392=f(-0.88000)*/
{14, 0,123,__LINE__, 0xbfe92a32, 0xe0000000, 0xbfebd70a, 0x3d70a3d4},			/* -0.78640=f(-0.87000)*/
{14, 0,123,__LINE__, 0xbfe8ec3d, 0xc0000000, 0xbfeb851e, 0xb851eb82},			/* -0.77883=f(-0.86000)*/
{14, 0,123,__LINE__, 0xbfe8adfa, 0x20000000, 0xbfeb3333, 0x33333330},			/* -0.77123=f(-0.85000)*/
{14, 0,123,__LINE__, 0xbfe86f67, 0xa0000000, 0xbfeae147, 0xae147ade},			/* -0.76359=f(-0.84000)*/
{14, 0,123,__LINE__, 0xbfe83086, 0x00000000, 0xbfea8f5c, 0x28f5c28c},			/* -0.75592=f(-0.83000)*/
{12, 0,123,__LINE__, 0xbfe7f155, 0x40000000, 0xbfea3d70, 0xa3d70a3a},			/* -0.74820=f(-0.82000)*/
{12, 0,123,__LINE__, 0xbfe7b1d4, 0xc0000000, 0xbfe9eb85, 0x1eb851e8},			/* -0.74045=f(-0.81000)*/
{12, 0,123,__LINE__, 0xbfe77204, 0xa0000000, 0xbfe99999, 0x99999996},			/* -0.73266=f(-0.80000)*/
{12, 0,123,__LINE__, 0xbfe731e4, 0xc0000000, 0xbfe947ae, 0x147ae144},			/* -0.72484=f(-0.79000)*/
{12, 0,123,__LINE__, 0xbfe6f174, 0xa0000000, 0xbfe8f5c2, 0x8f5c28f2},			/* -0.71697=f(-0.78000)*/
{12, 0,123,__LINE__, 0xbfe6b0b4, 0x80000000, 0xbfe8a3d7, 0x0a3d70a0},			/* -0.70907=f(-0.77000)*/
{12, 0,123,__LINE__, 0xbfe66fa3, 0xe0000000, 0xbfe851eb, 0x851eb84e},			/* -0.70112=f(-0.76000)*/
{15, 0,123,__LINE__, 0xbfe62e43, 0x00000000, 0xbfe7ffff, 0xfffffffc},			/* -0.69314=f(-0.75000)*/
{14, 0,123,__LINE__, 0xbfe5ec91, 0x80000000, 0xbfe7ae14, 0x7ae147aa},			/* -0.68512=f(-0.74000)*/
{14, 0,123,__LINE__, 0xbfe5aa8f, 0x60000000, 0xbfe75c28, 0xf5c28f58},			/* -0.67707=f(-0.73000)*/
{14, 0,123,__LINE__, 0xbfe5683c, 0xa0000000, 0xbfe70a3d, 0x70a3d706},			/* -0.66897=f(-0.72000)*/
{14, 0,123,__LINE__, 0xbfe52599, 0x20000000, 0xbfe6b851, 0xeb851eb4},			/* -0.66083=f(-0.71000)*/
{14, 0,123,__LINE__, 0xbfe4e2a5, 0x00000000, 0xbfe66666, 0x66666662},			/* -0.65266=f(-0.70000)*/
{14, 0,123,__LINE__, 0xbfe49f60, 0x20000000, 0xbfe6147a, 0xe147ae10},			/* -0.64445=f(-0.69000)*/
{15, 0,123,__LINE__, 0xbfe45bca, 0x80000000, 0xbfe5c28f, 0x5c28f5be},			/* -0.63620=f(-0.68000)*/
{15, 0,123,__LINE__, 0xbfe417e4, 0x60000000, 0xbfe570a3, 0xd70a3d6c},			/* -0.62791=f(-0.67000)*/
{13, 0,123,__LINE__, 0xbfe3d3ad, 0xa0000000, 0xbfe51eb8, 0x51eb851a},			/* -0.61958=f(-0.66000)*/
{13, 0,123,__LINE__, 0xbfe38f26, 0x40000000, 0xbfe4cccc, 0xccccccc8},			/* -0.61122=f(-0.65000)*/
{13, 0,123,__LINE__, 0xbfe34a4e, 0xa0000000, 0xbfe47ae1, 0x47ae1476},			/* -0.60282=f(-0.64000)*/
{13, 0,123,__LINE__, 0xbfe30526, 0xe0000000, 0xbfe428f5, 0xc28f5c24},			/* -0.59437=f(-0.63000)*/
{15, 0,123,__LINE__, 0xbfe2bfaf, 0x20000000, 0xbfe3d70a, 0x3d70a3d2},			/* -0.58589=f(-0.62000)*/
{15, 0,123,__LINE__, 0xbfe279e7, 0x80000000, 0xbfe3851e, 0xb851eb80},			/* -0.57738=f(-0.61000)*/
{15, 0,123,__LINE__, 0xbfe233d0, 0x40000000, 0xbfe33333, 0x3333332e},			/* -0.56882=f(-0.60000)*/
{14, 0,123,__LINE__, 0xbfe1ed69, 0xa0000000, 0xbfe2e147, 0xae147adc},			/* -0.56023=f(-0.59000)*/
{14, 0,123,__LINE__, 0xbfe1a6b4, 0x20000000, 0xbfe28f5c, 0x28f5c28a},			/* -0.55159=f(-0.58000)*/
{14, 0,123,__LINE__, 0xbfe15faf, 0xc0000000, 0xbfe23d70, 0xa3d70a38},			/* -0.54293=f(-0.57000)*/
{16, 0,123,__LINE__, 0xbfe1185d, 0x00000000, 0xbfe1eb85, 0x1eb851e6},			/* -0.53422=f(-0.56000)*/
{15, 0,123,__LINE__, 0xbfe0d0bc, 0x60000000, 0xbfe19999, 0x99999994},			/* -0.52548=f(-0.55000)*/
{15, 0,123,__LINE__, 0xbfe088ce, 0x20000000, 0xbfe147ae, 0x147ae142},			/* -0.51669=f(-0.54000)*/
{16, 0,123,__LINE__, 0xbfe04092, 0xa0000000, 0xbfe0f5c2, 0x8f5c28f0},			/* -0.50788=f(-0.53000)*/
{10, 0,123,__LINE__, 0xbfdff014, 0xe0000000, 0xbfe0a3d7, 0x0a3d709e},			/* -0.49902=f(-0.52000)*/
{10, 0,123,__LINE__, 0xbfdf5e6c, 0x80000000, 0xbfe051eb, 0x851eb84c},			/* -0.49013=f(-0.51000)*/
{10, 0,123,__LINE__, 0xbfdecc2c, 0xc0000000, 0xbfdfffff, 0xfffffff4},			/* -0.48121=f(-0.50000)*/
{15, 0,123,__LINE__, 0xbfde3956, 0xc0000000, 0xbfdf5c28, 0xf5c28f50},			/* -0.47224=f(-0.49000)*/
{14, 0,123,__LINE__, 0xbfdda5ec, 0x20000000, 0xbfdeb851, 0xeb851eac},			/* -0.46325=f(-0.48000)*/
{14, 0,123,__LINE__, 0xbfdd11ee, 0x00000000, 0xbfde147a, 0xe147ae08},			/* -0.45421=f(-0.47000)*/
{15, 0,123,__LINE__, 0xbfdc7d5d, 0xe0000000, 0xbfdd70a3, 0xd70a3d64},			/* -0.44515=f(-0.46000)*/
{13, 0,123,__LINE__, 0xbfdbe83c, 0xe0000000, 0xbfdccccc, 0xccccccc0},			/* -0.43604=f(-0.45000)*/
{13, 0,123,__LINE__, 0xbfdb528c, 0xc0000000, 0xbfdc28f5, 0xc28f5c1c},			/* -0.42691=f(-0.44000)*/
{15, 0,123,__LINE__, 0xbfdabc4f, 0x40000000, 0xbfdb851e, 0xb851eb78},			/* -0.41774=f(-0.43000)*/
{16, 0,123,__LINE__, 0xbfda2585, 0xc0000000, 0xbfdae147, 0xae147ad4},			/* -0.40854=f(-0.42000)*/
{14, 0,123,__LINE__, 0xbfd98e32, 0x60000000, 0xbfda3d70, 0xa3d70a30},			/* -0.39930=f(-0.41000)*/
{15, 0,123,__LINE__, 0xbfd8f656, 0xc0000000, 0xbfd99999, 0x9999998c},			/* -0.39003=f(-0.40000)*/
{16, 0,123,__LINE__, 0xbfd85df4, 0xc0000000, 0xbfd8f5c2, 0x8f5c28e8},			/* -0.38073=f(-0.39000)*/
{12, 0,123,__LINE__, 0xbfd7c50e, 0xa0000000, 0xbfd851eb, 0x851eb844},			/* -0.37140=f(-0.38000)*/
{16, 0,123,__LINE__, 0xbfd72ba6, 0x20000000, 0xbfd7ae14, 0x7ae147a0},			/* -0.36203=f(-0.37000)*/
{15, 0,123,__LINE__, 0xbfd691bd, 0xc0000000, 0xbfd70a3d, 0x70a3d6fc},			/* -0.35264=f(-0.36000)*/
{14, 0,123,__LINE__, 0xbfd5f757, 0x80000000, 0xbfd66666, 0x66666658},			/* -0.34322=f(-0.35000)*/
{16, 0,123,__LINE__, 0xbfd55c76, 0x00000000, 0xbfd5c28f, 0x5c28f5b4},			/* -0.33376=f(-0.34000)*/
{15, 0,123,__LINE__, 0xbfd4c11b, 0x60000000, 0xbfd51eb8, 0x51eb8510},			/* -0.32428=f(-0.33000)*/
{17, 0,123,__LINE__, 0xbfd4254a, 0x20000000, 0xbfd47ae1, 0x47ae146c},			/* -0.31477=f(-0.32000)*/
{17, 0,123,__LINE__, 0xbfd38905, 0x20000000, 0xbfd3d70a, 0x3d70a3c8},			/* -0.30523=f(-0.31000)*/
{15, 0,123,__LINE__, 0xbfd2ec4e, 0xc0000000, 0xbfd33333, 0x33333324},			/* -0.29567=f(-0.30000)*/
{16, 0,123,__LINE__, 0xbfd24f29, 0xa0000000, 0xbfd28f5c, 0x28f5c280},			/* -0.28608=f(-0.29000)*/
{17, 0,123,__LINE__, 0xbfd1b199, 0x00000000, 0xbfd1eb85, 0x1eb851dc},			/* -0.27646=f(-0.28000)*/
{17, 0,123,__LINE__, 0xbfd1139f, 0x60000000, 0xbfd147ae, 0x147ae138},			/* -0.26682=f(-0.27000)*/
{16, 0,123,__LINE__, 0xbfd0753f, 0xe0000000, 0xbfd0a3d7, 0x0a3d7094},			/* -0.25715=f(-0.26000)*/
{11, 0,123,__LINE__, 0xbfcfacfb, 0x20000000, 0xbfcfffff, 0xffffffe0},			/* -0.24746=f(-0.25000)*/
{16, 0,123,__LINE__, 0xbfce6eb6, 0xe0000000, 0xbfceb851, 0xeb851e98},			/* -0.23775=f(-0.24000)*/
{17, 0,123,__LINE__, 0xbfcd2fb9, 0xc0000000, 0xbfcd70a3, 0xd70a3d50},			/* -0.22801=f(-0.23000)*/
{13, 0,123,__LINE__, 0xbfcbf009, 0xe0000000, 0xbfcc28f5, 0xc28f5c08},			/* -0.21826=f(-0.22000)*/
{17, 0,123,__LINE__, 0xbfcaafae, 0x40000000, 0xbfcae147, 0xae147ac0},			/* -0.20848=f(-0.21000)*/
{16, 0,123,__LINE__, 0xbfc96ead, 0x60000000, 0xbfc99999, 0x99999978},			/* -0.19869=f(-0.20000)*/
{17, 0,123,__LINE__, 0xbfc82d0e, 0x80000000, 0xbfc851eb, 0x851eb830},			/* -0.18887=f(-0.19000)*/
{15, 0,123,__LINE__, 0xbfc6ead8, 0x60000000, 0xbfc70a3d, 0x70a3d6e8},			/* -0.17904=f(-0.18000)*/
{17, 0,123,__LINE__, 0xbfc5a812, 0x40000000, 0xbfc5c28f, 0x5c28f5a0},			/* -0.16919=f(-0.17000)*/
{19, 0,123,__LINE__, 0xbfc464c3, 0xa0000000, 0xbfc47ae1, 0x47ae1458},			/* -0.15932=f(-0.16000)*/
{19, 0,123,__LINE__, 0xbfc320f3, 0xc0000000, 0xbfc33333, 0x33333310},			/* -0.14944=f(-0.15000)*/
{18, 0,123,__LINE__, 0xbfc1dcaa, 0x40000000, 0xbfc1eb85, 0x1eb851c8},			/* -0.13954=f(-0.14000)*/
{18, 0,123,__LINE__, 0xbfc097ee, 0x80000000, 0xbfc0a3d7, 0x0a3d7080},			/* -0.12963=f(-0.13000)*/
{19, 0,123,__LINE__, 0xbfbea591, 0x00000000, 0xbfbeb851, 0xeb851e71},			/* -0.11971=f(-0.12000)*/
{18, 0,123,__LINE__, 0xbfbc1a80, 0x20000000, 0xbfbc28f5, 0xc28f5be2},			/* -0.10977=f(-0.11000)*/
{19, 0,123,__LINE__, 0xbfb98eb9, 0xe0000000, 0xbfb99999, 0x99999953},			/* -0.09983=f(-0.00100)*/
{20, 0,123,__LINE__, 0xbfb7024e, 0x80000000, 0xbfb70a3d, 0x70a3d6c4},			/* -0.08987=f(-0.09000)*/
{20, 0,123,__LINE__, 0xbfb4754d, 0xc0000000, 0xbfb47ae1, 0x47ae1435},			/* -0.07991=f(-0.08000)*/
{20, 0,123,__LINE__, 0xbfb1e7c8, 0x20000000, 0xbfb1eb85, 0x1eb851a6},			/* -0.06994=f(-0.07000)*/
{20, 0,123,__LINE__, 0xbfaeb39b, 0xe0000000, 0xbfaeb851, 0xeb851e2d},			/* -0.05996=f(-0.06000)*/
{20, 0,123,__LINE__, 0xbfa996df, 0x60000000, 0xbfa99999, 0x9999990e},			/* -0.04997=f(-0.05000)*/
{22, 0,123,__LINE__, 0xbfa4797b, 0xa0000000, 0xbfa47ae1, 0x47ae13ef},			/* -0.03998=f(-0.04000)*/
{20, 0,123,__LINE__, 0xbf9eb724, 0x00000000, 0xbf9eb851, 0xeb851da0},			/* -0.02999=f(-0.03000)*/
{25, 0,123,__LINE__, 0xbf947a87, 0xc0000000, 0xbf947ae1, 0x47ae1362},			/* -0.01999=f(-0.02000)*/
{26, 0,123,__LINE__, 0xbf847aca, 0xe0000000, 0xbf847ae1, 0x47ae1249},			/* -0.00999=f(-0.00010)*/
{64, 0,123,__LINE__, 0x3cd19000, 0x00000000, 0x3cd19000, 0x00000000},			/* 9.74915e-16=f(9.74915e-16)*/
{26, 0,123,__LINE__, 0x3f847aca, 0xe0000000, 0x3f847ae1, 0x47ae16ad},			/* 0.00999=f(0.01000)*/
{25, 0,123,__LINE__, 0x3f947a87, 0xc0000000, 0x3f947ae1, 0x47ae1594},			/* 0.01999=f(0.02000)*/
{20, 0,123,__LINE__, 0x3f9eb724, 0x00000000, 0x3f9eb851, 0xeb851fd2},			/* 0.02999=f(0.03000)*/
{22, 0,123,__LINE__, 0x3fa4797b, 0xa0000000, 0x3fa47ae1, 0x47ae1508},			/* 0.03998=f(0.04000)*/
{20, 0,123,__LINE__, 0x3fa996df, 0x60000000, 0x3fa99999, 0x99999a27},			/* 0.04997=f(0.05000)*/
{20, 0,123,__LINE__, 0x3faeb39b, 0xe0000000, 0x3faeb851, 0xeb851f46},			/* 0.05996=f(0.06000)*/
{20, 0,123,__LINE__, 0x3fb1e7c8, 0x20000000, 0x3fb1eb85, 0x1eb85232},			/* 0.06994=f(0.07000)*/
{20, 0,123,__LINE__, 0x3fb4754d, 0xc0000000, 0x3fb47ae1, 0x47ae14c1},			/* 0.07991=f(0.08000)*/
{20, 0,123,__LINE__, 0x3fb7024e, 0x80000000, 0x3fb70a3d, 0x70a3d750},			/* 0.08987=f(0.09000)*/
{19, 0,123,__LINE__, 0x3fb98eb9, 0xe0000000, 0x3fb99999, 0x999999df},			/* 0.09983=f(0.10000)*/
{18, 0,123,__LINE__, 0x3fbc1a80, 0x20000000, 0x3fbc28f5, 0xc28f5c6e},			/* 0.10977=f(0.11000)*/
{19, 0,123,__LINE__, 0x3fbea591, 0x00000000, 0x3fbeb851, 0xeb851efd},			/* 0.11971=f(0.12000)*/
{18, 0,123,__LINE__, 0x3fc097ee, 0x80000000, 0x3fc0a3d7, 0x0a3d70c6},			/* 0.12963=f(0.13000)*/
{18, 0,123,__LINE__, 0x3fc1dcaa, 0x40000000, 0x3fc1eb85, 0x1eb8520e},			/* 0.13954=f(0.14000)*/
{19, 0,123,__LINE__, 0x3fc320f3, 0xc0000000, 0x3fc33333, 0x33333356},			/* 0.14944=f(0.15000)*/
{19, 0,123,__LINE__, 0x3fc464c3, 0xa0000000, 0x3fc47ae1, 0x47ae149e},			/* 0.15932=f(0.16000)*/
{17, 0,123,__LINE__, 0x3fc5a812, 0x40000000, 0x3fc5c28f, 0x5c28f5e6},			/* 0.16919=f(0.17000)*/
{15, 0,123,__LINE__, 0x3fc6ead8, 0x60000000, 0x3fc70a3d, 0x70a3d72e},			/* 0.17904=f(0.18000)*/
{17, 0,123,__LINE__, 0x3fc82d0e, 0x80000000, 0x3fc851eb, 0x851eb876},			/* 0.18887=f(0.19000)*/
{16, 0,123,__LINE__, 0x3fc96ead, 0x60000000, 0x3fc99999, 0x999999be},			/* 0.19869=f(0.20000)*/
{17, 0,123,__LINE__, 0x3fcaafae, 0x40000000, 0x3fcae147, 0xae147b06},			/* 0.20848=f(0.21000)*/
{13, 0,123,__LINE__, 0x3fcbf009, 0xe0000000, 0x3fcc28f5, 0xc28f5c4e},			/* 0.21826=f(0.22000)*/
{17, 0,123,__LINE__, 0x3fcd2fb9, 0xc0000000, 0x3fcd70a3, 0xd70a3d96},			/* 0.22801=f(0.23000)*/
{16, 0,123,__LINE__, 0x3fce6eb6, 0xe0000000, 0x3fceb851, 0xeb851ede},			/* 0.23775=f(0.24000)*/
{11, 0,123,__LINE__, 0x3fcfacfb, 0x20000000, 0x3fd00000, 0x00000013},			/* 0.24746=f(0.25000)*/
{16, 0,123,__LINE__, 0x3fd0753f, 0xe0000000, 0x3fd0a3d7, 0x0a3d70b7},			/* 0.25715=f(0.26000)*/
{17, 0,123,__LINE__, 0x3fd1139f, 0x60000000, 0x3fd147ae, 0x147ae15b},			/* 0.26682=f(0.27000)*/
{17, 0,123,__LINE__, 0x3fd1b199, 0x00000000, 0x3fd1eb85, 0x1eb851ff},			/* 0.27646=f(0.28000)*/
{16, 0,123,__LINE__, 0x3fd24f29, 0xa0000000, 0x3fd28f5c, 0x28f5c2a3},			/* 0.28608=f(0.29000)*/
{15, 0,123,__LINE__, 0x3fd2ec4e, 0xc0000000, 0x3fd33333, 0x33333347},			/* 0.29567=f(0.30000)*/
{17, 0,123,__LINE__, 0x3fd38905, 0x20000000, 0x3fd3d70a, 0x3d70a3eb},			/* 0.30523=f(0.31000)*/
{17, 0,123,__LINE__, 0x3fd4254a, 0x20000000, 0x3fd47ae1, 0x47ae148f},			/* 0.31477=f(0.32000)*/
{15, 0,123,__LINE__, 0x3fd4c11b, 0x60000000, 0x3fd51eb8, 0x51eb8533},			/* 0.32428=f(0.33000)*/
{16, 0,123,__LINE__, 0x3fd55c76, 0x00000000, 0x3fd5c28f, 0x5c28f5d7},			/* 0.33376=f(0.34000)*/
{14, 0,123,__LINE__, 0x3fd5f757, 0x80000000, 0x3fd66666, 0x6666667b},			/* 0.34322=f(0.35000)*/
{15, 0,123,__LINE__, 0x3fd691bd, 0xc0000000, 0x3fd70a3d, 0x70a3d71f},			/* 0.35264=f(0.36000)*/
{16, 0,123,__LINE__, 0x3fd72ba6, 0x20000000, 0x3fd7ae14, 0x7ae147c3},			/* 0.36203=f(0.37000)*/
{12, 0,123,__LINE__, 0x3fd7c50e, 0xa0000000, 0x3fd851eb, 0x851eb867},			/* 0.37140=f(0.38000)*/
{16, 0,123,__LINE__, 0x3fd85df4, 0xc0000000, 0x3fd8f5c2, 0x8f5c290b},			/* 0.38073=f(0.39000)*/
{15, 0,123,__LINE__, 0x3fd8f656, 0xc0000000, 0x3fd99999, 0x999999af},			/* 0.39003=f(0.40000)*/
{14, 0,123,__LINE__, 0x3fd98e32, 0x60000000, 0x3fda3d70, 0xa3d70a53},			/* 0.39930=f(0.41000)*/
{16, 0,123,__LINE__, 0x3fda2585, 0xc0000000, 0x3fdae147, 0xae147af7},			/* 0.40854=f(0.42000)*/
{15, 0,123,__LINE__, 0x3fdabc4f, 0x40000000, 0x3fdb851e, 0xb851eb9b},			/* 0.41774=f(0.43000)*/
{13, 0,123,__LINE__, 0x3fdb528c, 0xc0000000, 0x3fdc28f5, 0xc28f5c3f},			/* 0.42691=f(0.44000)*/
{13, 0,123,__LINE__, 0x3fdbe83c, 0xe0000000, 0x3fdccccc, 0xcccccce3},			/* 0.43604=f(0.45000)*/
{15, 0,123,__LINE__, 0x3fdc7d5d, 0xe0000000, 0x3fdd70a3, 0xd70a3d87},			/* 0.44515=f(0.46000)*/
{14, 0,123,__LINE__, 0x3fdd11ee, 0x00000000, 0x3fde147a, 0xe147ae2b},			/* 0.45421=f(0.47000)*/
{14, 0,123,__LINE__, 0x3fdda5ec, 0x20000000, 0x3fdeb851, 0xeb851ecf},			/* 0.46325=f(0.48000)*/
{15, 0,123,__LINE__, 0x3fde3956, 0xc0000000, 0x3fdf5c28, 0xf5c28f73},			/* 0.47224=f(0.49000)*/
{10, 0,123,__LINE__, 0x3fdecc2c, 0xc0000000, 0x3fe00000, 0x0000000b},			/* 0.48121=f(0.50000)*/
{10, 0,123,__LINE__, 0x3fdf5e6c, 0x80000000, 0x3fe051eb, 0x851eb85d},			/* 0.49013=f(0.51000)*/
{10, 0,123,__LINE__, 0x3fdff014, 0xe0000000, 0x3fe0a3d7, 0x0a3d70af},			/* 0.49902=f(0.52000)*/
{16, 0,123,__LINE__, 0x3fe04092, 0xa0000000, 0x3fe0f5c2, 0x8f5c2901},			/* 0.50788=f(0.53000)*/
{15, 0,123,__LINE__, 0x3fe088ce, 0x20000000, 0x3fe147ae, 0x147ae153},			/* 0.51669=f(0.54000)*/
{15, 0,123,__LINE__, 0x3fe0d0bc, 0x60000000, 0x3fe19999, 0x999999a5},			/* 0.52548=f(0.55000)*/
{16, 0,123,__LINE__, 0x3fe1185d, 0x00000000, 0x3fe1eb85, 0x1eb851f7},			/* 0.53422=f(0.56000)*/
{14, 0,123,__LINE__, 0x3fe15faf, 0xc0000000, 0x3fe23d70, 0xa3d70a49},			/* 0.54293=f(0.57000)*/
{14, 0,123,__LINE__, 0x3fe1a6b4, 0x20000000, 0x3fe28f5c, 0x28f5c29b},			/* 0.55159=f(0.58000)*/
{14, 0,123,__LINE__, 0x3fe1ed69, 0xa0000000, 0x3fe2e147, 0xae147aed},			/* 0.56023=f(0.59000)*/
{15, 0,123,__LINE__, 0x3fe233d0, 0x40000000, 0x3fe33333, 0x3333333f},			/* 0.56882=f(0.60000)*/
{15, 0,123,__LINE__, 0x3fe279e7, 0x80000000, 0x3fe3851e, 0xb851eb91},			/* 0.57738=f(0.61000)*/
{15, 0,123,__LINE__, 0x3fe2bfaf, 0x20000000, 0x3fe3d70a, 0x3d70a3e3},			/* 0.58589=f(0.62000)*/
{13, 0,123,__LINE__, 0x3fe30526, 0xe0000000, 0x3fe428f5, 0xc28f5c35},			/* 0.59437=f(0.63000)*/
{13, 0,123,__LINE__, 0x3fe34a4e, 0xa0000000, 0x3fe47ae1, 0x47ae1487},			/* 0.60282=f(0.64000)*/
{13, 0,123,__LINE__, 0x3fe38f26, 0x40000000, 0x3fe4cccc, 0xccccccd9},			/* 0.61122=f(0.65000)*/
{13, 0,123,__LINE__, 0x3fe3d3ad, 0xa0000000, 0x3fe51eb8, 0x51eb852b},			/* 0.61958=f(0.66000)*/
{15, 0,123,__LINE__, 0x3fe417e4, 0x60000000, 0x3fe570a3, 0xd70a3d7d},			/* 0.62791=f(0.67000)*/
{15, 0,123,__LINE__, 0x3fe45bca, 0x80000000, 0x3fe5c28f, 0x5c28f5cf},			/* 0.63620=f(0.68000)*/
{14, 0,123,__LINE__, 0x3fe49f60, 0x20000000, 0x3fe6147a, 0xe147ae21},			/* 0.64445=f(0.69000)*/
{14, 0,123,__LINE__, 0x3fe4e2a5, 0x00000000, 0x3fe66666, 0x66666673},			/* 0.65266=f(0.70000)*/
{14, 0,123,__LINE__, 0x3fe52599, 0x20000000, 0x3fe6b851, 0xeb851ec5},			/* 0.66083=f(0.71000)*/
{14, 0,123,__LINE__, 0x3fe5683c, 0xa0000000, 0x3fe70a3d, 0x70a3d717},			/* 0.66897=f(0.72000)*/
{14, 0,123,__LINE__, 0x3fe5aa8f, 0x60000000, 0x3fe75c28, 0xf5c28f69},			/* 0.67707=f(0.73000)*/
{14, 0,123,__LINE__, 0x3fe5ec91, 0x80000000, 0x3fe7ae14, 0x7ae147bb},			/* 0.68512=f(0.74000)*/
{15, 0,123,__LINE__, 0x3fe62e43, 0x00000000, 0x3fe80000, 0x0000000d},			/* 0.69314=f(0.75000)*/
{12, 0,123,__LINE__, 0x3fe66fa3, 0xe0000000, 0x3fe851eb, 0x851eb85f},			/* 0.70112=f(0.76000)*/
{12, 0,123,__LINE__, 0x3fe6b0b4, 0x80000000, 0x3fe8a3d7, 0x0a3d70b1},			/* 0.70907=f(0.77000)*/
{12, 0,123,__LINE__, 0x3fe6f174, 0xa0000000, 0x3fe8f5c2, 0x8f5c2903},			/* 0.71697=f(0.78000)*/
{12, 0,123,__LINE__, 0x3fe731e4, 0xc0000000, 0x3fe947ae, 0x147ae155},			/* 0.72484=f(0.79000)*/
{12, 0,123,__LINE__, 0x3fe77204, 0xa0000000, 0x3fe99999, 0x999999a7},			/* 0.73266=f(0.80000)*/
{12, 0,123,__LINE__, 0x3fe7b1d4, 0xc0000000, 0x3fe9eb85, 0x1eb851f9},			/* 0.74045=f(0.81000)*/
{12, 0,123,__LINE__, 0x3fe7f155, 0x40000000, 0x3fea3d70, 0xa3d70a4b},			/* 0.74820=f(0.82000)*/
{14, 0,123,__LINE__, 0x3fe83086, 0x00000000, 0x3fea8f5c, 0x28f5c29d},			/* 0.75592=f(0.83000)*/
{14, 0,123,__LINE__, 0x3fe86f67, 0xa0000000, 0x3feae147, 0xae147aef},			/* 0.76359=f(0.84000)*/
{14, 0,123,__LINE__, 0x3fe8adfa, 0x20000000, 0x3feb3333, 0x33333341},			/* 0.77123=f(0.85000)*/
{14, 0,123,__LINE__, 0x3fe8ec3d, 0xc0000000, 0x3feb851e, 0xb851eb93},			/* 0.77883=f(0.86000)*/
{14, 0,123,__LINE__, 0x3fe92a32, 0xe0000000, 0x3febd70a, 0x3d70a3e5},			/* 0.78640=f(0.87000)*/
{13, 0,123,__LINE__, 0x3fe967d9, 0x80000000, 0x3fec28f5, 0xc28f5c37},			/* 0.79392=f(0.88000)*/
{13, 0,123,__LINE__, 0x3fe9a532, 0x20000000, 0x3fec7ae1, 0x47ae1489},			/* 0.80141=f(0.89000)*/
{13, 0,123,__LINE__, 0x3fe9e23c, 0xe0000000, 0x3feccccc, 0xccccccdb},			/* 0.80886=f(0.90000)*/
{13, 0,123,__LINE__, 0x3fea1efa, 0x40000000, 0x3fed1eb8, 0x51eb852d},			/* 0.81628=f(0.91000)*/
{13, 0,123,__LINE__, 0x3fea5b6a, 0x40000000, 0x3fed70a3, 0xd70a3d7f},			/* 0.82365=f(0.92000)*/
{13, 0,123,__LINE__, 0x3fea978d, 0x80000000, 0x3fedc28f, 0x5c28f5d1},			/* 0.83100=f(0.93000)*/
{13, 0,123,__LINE__, 0x3fead364, 0x00000000, 0x3fee147a, 0xe147ae23},			/* 0.83830=f(0.94000)*/
{13, 0,123,__LINE__, 0x3feb0eee, 0x60000000, 0x3fee6666, 0x66666675},			/* 0.84557=f(0.95000)*/
{13, 0,123,__LINE__, 0x3feb4a2c, 0xe0000000, 0x3feeb851, 0xeb851ec7},			/* 0.85280=f(0.96000)*/
{13, 0,123,__LINE__, 0x3feb851f, 0xc0000000, 0x3fef0a3d, 0x70a3d719},			/* 0.86000=f(0.97000)*/
{13, 0,123,__LINE__, 0x3febbfc7, 0x80000000, 0x3fef5c28, 0xf5c28f6b},			/* 0.86716=f(0.98000)*/
{13, 0,123,__LINE__, 0x3febfa24, 0x20000000, 0x3fefae14, 0x7ae147bd},			/* 0.87428=f(0.99000)*/
{14, 0,123,__LINE__, 0x3fec3436, 0x60000000, 0x3ff00000, 0x00000007},			/* 0.88137=f(1.00000)*/
{11, 0,123,__LINE__, 0x3fec6dfe, 0x60000000, 0x3ff028f5, 0xc28f5c30},			/* 0.88842=f(1.01000)*/
{11, 0,123,__LINE__, 0x3feca77c, 0xc0000000, 0x3ff051eb, 0x851eb859},			/* 0.89544=f(1.02000)*/
{11, 0,123,__LINE__, 0x3fece0b1, 0xa0000000, 0x3ff07ae1, 0x47ae1482},			/* 0.90242=f(1.03000)*/
{11, 0,123,__LINE__, 0x3fed199d, 0xa0000000, 0x3ff0a3d7, 0x0a3d70ab},			/* 0.90937=f(1.04000)*/
{11, 0,123,__LINE__, 0x3fed5240, 0xe0000000, 0x3ff0cccc, 0xccccccd4},			/* 0.91629=f(1.05000)*/
{11, 0,123,__LINE__, 0x3fed8a9b, 0xe0000000, 0x3ff0f5c2, 0x8f5c28fd},			/* 0.92317=f(1.06000)*/
{11, 0,123,__LINE__, 0x3fedc2af, 0x40000000, 0x3ff11eb8, 0x51eb8526},			/* 0.93001=f(1.07000)*/
{11, 0,123,__LINE__, 0x3fedfa7b, 0x20000000, 0x3ff147ae, 0x147ae14f},			/* 0.93682=f(1.08000)*/
{11, 0,123,__LINE__, 0x3fee31ff, 0xe0000000, 0x3ff170a3, 0xd70a3d78},			/* 0.94360=f(1.09000)*/
{11, 0,123,__LINE__, 0x3fee693d, 0xe0000000, 0x3ff19999, 0x999999a1},			/* 0.95034=f(1.10000)*/
{11, 0,123,__LINE__, 0x3feea035, 0xe0000000, 0x3ff1c28f, 0x5c28f5ca},			/* 0.95705=f(1.11000)*/
{11, 0,123,__LINE__, 0x3feed6e8, 0x00000000, 0x3ff1eb85, 0x1eb851f3},			/* 0.96373=f(1.12000)*/
{11, 0,123,__LINE__, 0x3fef0d54, 0xc0000000, 0x3ff2147a, 0xe147ae1c},			/* 0.97037=f(1.13000)*/
{11, 0,123,__LINE__, 0x3fef437c, 0x80000000, 0x3ff23d70, 0xa3d70a45},			/* 0.97698=f(1.14000)*/
{11, 0,123,__LINE__, 0x3fef795f, 0xa0000000, 0x3ff26666, 0x6666666e},			/* 0.98356=f(1.15000)*/
{11, 0,123,__LINE__, 0x3fefaefe, 0xc0000000, 0x3ff28f5c, 0x28f5c297},			/* 0.99011=f(1.16000)*/
{11, 0,123,__LINE__, 0x3fefe45a, 0x00000000, 0x3ff2b851, 0xeb851ec0},			/* 0.99662=f(1.17000)*/
{14, 0,123,__LINE__, 0x3ff00cb9, 0x00000000, 0x3ff2e147, 0xae147ae9},			/* 1.00310=f(1.18000)*/
{14, 0,123,__LINE__, 0x3ff02723, 0xa0000000, 0x3ff30a3d, 0x70a3d712},			/* 1.00955=f(1.19000)*/
{16, 0,123,__LINE__, 0xc0044c62, 0xa0000000, 0xc01921fb, 0x54442d18},			/* -2.53729=f(-6.28318)*/
{15, 0,123,__LINE__, 0xc002090a, 0x80000000, 0xc012d97c, 0x7f3321d2},			/* -2.25441=f(-4.71238)*/
{ 1, 0,123,__LINE__, 0xbffdcbf6, 0xa0000000, 0xc00921fb, 0x54442d18},			/* -1.86229=f(-3.14159)*/
{12, 0,123,__LINE__, 0xbff3bc04, 0xe0000000, 0xbff921fb, 0x54442d18},			/* -1.23340=f(-1.57079)*/
{64, 0,123,__LINE__, 0x00000000, 0x00000000, 0x00000000, 0x00000000},			/* 0.00000=f(0.00000)*/
{12, 0,123,__LINE__, 0x3ff3bc04, 0xe0000000, 0x3ff921fb, 0x54442d18},			/* 1.23340=f(1.57079)*/
{ 1, 0,123,__LINE__, 0x3ffdcbf6, 0xa0000000, 0x400921fb, 0x54442d18},			/* 1.86229=f(3.14159)*/
{15, 0,123,__LINE__, 0x4002090a, 0x80000000, 0x4012d97c, 0x7f3321d2},			/* 2.25441=f(4.71238)*/
{11, 0,123,__LINE__, 0xc01060e4, 0xa0000000, 0xc03e0000, 0x00000000},			/* -4.09462=f(-30.0000)*/
{11, 0,123,__LINE__, 0xc0102531, 0x60000000, 0xc03c4ccc, 0xcccccccd},			/* -4.03632=f(-28.3000)*/
{12, 0,123,__LINE__, 0xc00fcb98, 0x40000000, 0xc03a9999, 0x9999999a},			/* -3.97441=f(-26.6000)*/
{12, 0,123,__LINE__, 0xc00f4470, 0xa0000000, 0xc038e666, 0x66666667},			/* -3.90841=f(-24.9000)*/
{12, 0,123,__LINE__, 0xc00eb3bd, 0x60000000, 0xc0373333, 0x33333334},			/* -3.83776=f(-23.2000)*/
{12, 0,123,__LINE__, 0xc00e180b, 0x60000000, 0xc0358000, 0x00000001},			/* -3.76174=f(-21.5000)*/
{12, 0,123,__LINE__, 0xc00d6f8b, 0xe0000000, 0xc033cccc, 0xccccccce},			/* -3.67946=f(-19.8000)*/
{12, 0,123,__LINE__, 0xc00cb7f4, 0x40000000, 0xc0321999, 0x9999999b},			/* -3.58982=f(-18.1000)*/
{12, 0,123,__LINE__, 0xc00bee4c, 0x80000000, 0xc0306666, 0x66666668},			/* -3.49135=f(-16.4000)*/
{12, 0,123,__LINE__, 0xc00b0ea4, 0x60000000, 0xc02d6666, 0x6666666a},			/* -3.38214=f(-14.7000)*/
{12, 0,123,__LINE__, 0xc00a139a, 0xc0000000, 0xc02a0000, 0x00000004},			/* -3.25957=f(-13.0000)*/
{12, 0,123,__LINE__, 0xc008f58f, 0x40000000, 0xc0269999, 0x9999999e},			/* -3.11990=f(-11.3000)*/
{14, 0,123,__LINE__, 0xc007a930, 0x80000000, 0xc0233333, 0x33333338},			/* -2.95761=f(-9.60000)*/
{14, 0,123,__LINE__, 0xc0061ca7, 0xc0000000, 0xc01f9999, 0x999999a3},			/* -2.76399=f(-7.90000)*/
{16, 0,123,__LINE__, 0xc004316f, 0x00000000, 0xc018cccc, 0xccccccd6},			/* -2.52413=f(-6.20000)*/
{14, 0,123,__LINE__, 0xc001acbe, 0x80000000, 0xc0120000, 0x00000009},			/* -2.20934=f(-4.50000)*/
{ 1, 0,123,__LINE__, 0xbffc0d39, 0xa0000000, 0xc0066666, 0x66666678},			/* -1.75322=f(-2.80000)*/
{11, 0,123,__LINE__, 0xbfee693d, 0xe0000000, 0xbff19999, 0x999999bd},			/* -0.95034=f(-1.10000)*/
{15, 0,123,__LINE__, 0x3fe233d0, 0x40000000, 0x3fe33333, 0x333332ec},			/* 0.56882=f(0.60000)*/
{ 1, 0,123,__LINE__, 0x3ff91fdc, 0x60000000, 0x40026666, 0x66666654},			/* 1.57027=f(2.30000)*/
{14, 0,123,__LINE__, 0x4000c1f8, 0xa0000000, 0x400fffff, 0xffffffee},			/* 2.09471=f(4.00000)*/
{13, 0,123,__LINE__, 0x4003879e, 0xc0000000, 0x4016cccc, 0xccccccc4},			/* 2.44122=f(5.70000)*/
{15, 0,123,__LINE__, 0x400597e2, 0x00000000, 0x401d9999, 0x99999991},			/* 2.69916=f(7.40000)*/
{14, 0,123,__LINE__, 0x40073c44, 0x40000000, 0x40223333, 0x3333332f},			/* 2.90442=f(9.10000)*/
{12, 0,123,__LINE__, 0x40089940, 0x60000000, 0x40259999, 0x99999995},			/* 3.07483=f(10.8000)*/
{12, 0,123,__LINE__, 0x4009c386, 0xe0000000, 0x4028ffff, 0xfffffffb},			/* 3.22047=f(12.5000)*/
{12, 0,123,__LINE__, 0x400ac7f0, 0x80000000, 0x402c6666, 0x66666661},			/* 3.34762=f(14.2000)*/
{12, 0,123,__LINE__, 0x400baf02, 0x60000000, 0x402fcccc, 0xccccccc7},			/* 3.46045=f(15.9000)*/
{12, 0,123,__LINE__, 0x400c7eac, 0x60000000, 0x40319999, 0x99999997},			/* 3.56185=f(17.6000)*/
{12, 0,123,__LINE__, 0x400d3b3b, 0xe0000000, 0x40334ccc, 0xccccccca},			/* 3.65392=f(19.3000)*/
{12, 0,123,__LINE__, 0x400de7e8, 0x40000000, 0x4034ffff, 0xfffffffd},			/* 3.73823=f(21.0000)*/
{12, 0,123,__LINE__, 0x400e8729, 0x60000000, 0x4036b333, 0x33333330},			/* 3.81599=f(22.7000)*/
{12, 0,123,__LINE__, 0x400f1aee, 0x60000000, 0x40386666, 0x66666663},			/* 3.88815=f(24.4000)*/
{12, 0,123,__LINE__, 0x400fa4c2, 0xa0000000, 0x403a1999, 0x99999996},			/* 3.95544=f(26.1000)*/
{11, 0,123,__LINE__, 0x401012f3, 0x60000000, 0x403bcccc, 0xccccccc9},			/* 4.01850=f(27.8000)*/
{11, 0,123,__LINE__, 0x40104fb1, 0x40000000, 0x403d7fff, 0xfffffffc},			/* 4.07782=f(29.5000)*/
0,};
test_asinhf(m)   {run_vector_1(m,asinhf_vec,(char *)(asinhf),"asinhf","ff");   }
