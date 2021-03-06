/*
 * Copyright (c) 2010 SURFnet bv
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*****************************************************************************
 HashTests.cpp

 Contains test cases to test the hash implementations
 *****************************************************************************/

#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>
#include "HashTests.h"
#include "CryptoFactory.h"
#include <stdio.h>
#include "HashAlgorithm.h"
#include "RNG.h"

CPPUNIT_TEST_SUITE_REGISTRATION(HashTests);

void HashTests::setUp()
{
	hash = NULL;
	rng = NULL;
}

void HashTests::tearDown()
{
	if (hash != NULL)
	{
		CryptoFactory::i()->recycleHashAlgorithm(hash);
	}

	fflush(stdout);
}

#ifndef WITH_FIPS
void HashTests::testMD5()
{
	char testData[4096] = "EDB39CC3CB5E8FD23A2182FAE8781F4E8E8534C8FE10BDC7ACDB34C4E22E4B60872E51AD092950771E475FB97B05D6203A0D1B3326D9678E44FB40787D8CF16BD3BBE4D4786F60BD8E9766EF1C30B22301FA8F7232358B3C5BE644342499622A5153B29D2E5F8F509771E8A5DE3B3737D30F2AB035262B8D83EC55F683E9B62BBB31E2E1F5E142A13ACB9701E1DE8752ACD492A943C2DB57CF4035BDA9D0519B86847CF74DB2F43CC23016D88EF44BA422710C00AEBB2ABDDED89D92E4BD132CEB87581644198B0D4AF82F3CE161ED03A4E189BE8276243243817F91159BDE294E61400041537618703754C609709846C3EDAE6922B498D24FDC6B9AE702A034BB3A2C6C230E58EE5759DC84AD0B960B111153F6D619E850E36833A26689B4718719EFF3A407B77B0BAD24A17E29B6A29E9125B4CF0DFFEB8D0B03D533ADA43D1F20CFFE0A067E0F6FFF28510148DD72AE33D8B72227523497B84082B682A480FA3B2FE42AAFF49883DAA4DC6826C9593D38002D73FA7AA91B41E828CC34FF12ECC1452B73709696A440CCDF3E6116DA36CB7A01C9E168C3518304721D2CDBC077F4D4B7E8D1A59F954872862F72AC12CB16E33BED8EC3CFD912E36BBF4DA86E31F5F45BE9294550965C7CBC03518AD7C5637A97F7470EDD6E359E82815230CE03AEE9512939A214D017719E5F6B2487E3B468D50DC8EA41EEE94360E77E0EAA9C27DFE2C28D63B7699406E2E6620FE23F96584EEA04034270A2A44096B70017CC2CB197CE8DA3B1924D8D2E86FB40FF59CCFC78B3A4F543068F08C3406DEF52413F95B2BE2AD5DA1ADBB7D95F2133F61AD5518A1BEA018E3C3C0C154EAE56B0B0F4A8818AE47DC618D138245192D055CE4EF8BD1A75030EABAFD66E45B204BA6B123FF54D78F51B6A7DB9B08BEE93E923F9DE553EC0BE4B8BE222998BC792002005ED66C0218902E7DDF6C6D5043A8A9790E438D5BDE37791CDC0DB6C974638884270D4F379DF845281C5F1F8A20978086A9444779D59423B6CEF8175E0BC2C517FA921EC66BE3F11ECEE078553F610D5B65FD293E37717C0CB323A2760646AD56C77A4B96940CF5AD1154A642043E40788C2D367123399FA6015422F5EFEC851EF558ACD4BC31EF3D3EF912241A586E54B97FAE083EE45FBBBED74BC9E3887795FC2EBC5354ACA358EC509F4AB8B2967488D5E5B77DD44EB72E66694A1F490ADEB2848918531B6FA7216C51CDB507598F1AA0985626482D3546BEF5CA3BB409934D11AECBAD47389F2CBCD85FB4DD1A8A6F4D0C28920671A31FFC14FC00FC51296A6182D5A05B16B33232798F0993C78BCC662932E2F57626D639DD00E39B206C1F8A15A19B5DCA9F8B015611AFA47A10A30527F552A22574E15E3577EAD9C7B89782F82D7082D747B50CEFD38C6F17D6269A696F518EE6EAFE1E1EA96FB4D";
	char testResult[512] = "7299FE513582D71A3B9EA1E9F45F95FD";

	// Get a MD5 hash instance
	CPPUNIT_ASSERT((hash = CryptoFactory::i()->getHashAlgorithm(HashAlgo::MD5)) != NULL);

	ByteString b(testData);
	ByteString osslHash(testResult), shsmHash;

	// Now recreate the hash using our implementation in a single operation
	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	// Now recreate the hash in a multiple part operation
	shsmHash.wipe();

	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(0, 567)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567, 989)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567 + 989)));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	CryptoFactory::i()->recycleHashAlgorithm(hash);

	hash = NULL;
	rng = NULL;
}
#endif

void HashTests::testSHA1()
{
	char testData[4096] = "0583243FDEDEBF8C9EA4017808B490A2581A5A06B8C2CCA4F2259B7FABDE4BECA9F80C3EFBFEC436CF5F252568CDF55C7D31002456F6FE711D404F9A274037E8D55EC6C55077E4900EEAF6524C44FBFCC1C5C1C6AF962CE22106610A7C8DE7D725BF924B81EA566B952C8723E0EAFA21EB2C6E6F5528A89FC24C478C0600CB4FD482FAFF8507284C5F1E8AEBA9CDBFB3E71D77B775DD7C09E65049423BBAA70499450AE3B46703A4A44AE281C1F0341EDF0473B079C44E0F9EFB4C3683ED706E480C901A16E77B52030A7E46E31991131AED9C98DCA1850A4BD6F35DBCF5A894A8843EE656EF0DC617D923EB38426855FAA2E77E70CE9A8AC81D0F14259A748F9227F732420CDAE5A1CFEA9B541F0B12ADD5D966EBF66613D7FBAF5FFCAC8D625CFA536DBCA8E54B01B237211656F165A8114CD157E82870BCCE6AE8AF5334105B4E50B0A4E4AE5D0A3CC5CD8FF85F7C4D20D2627694B168CB48DB81B5B78C620C7559B2043C268F5D7351256D41FE892ED8EF6F7B5DD8E193EDCD268C55C209DCD0E827FCF76D6951CEBD0065826FBF044ED5818FEE4ECB99973FDB6826BF92488652E804C40D07D4CE43FCB31481667EE637721B91FD38331A98A6E8969D5B1D0690CFA0B500D72BF2D42AD6471FE064496C91126EFCC46B8CD093BCBB70D21FDE1937A888CA08DF3068A858280ABEAF71608BC9229E8E5EFF1814C4887E3A26782A110F347560DAB6B07DB8D063E8343A3FCD99B34C3C3F649096339CA7D39DA6F7B2E489A9EE6CE01C923D45C50350AC57247838E323F1B2A284606E45D60938A6F492545C5EBA0C7396702C0F4E77F36AC9ECE7ADD32FA4EEF7B38F9FF3F8BFB4E05F1025AAB52FB1B4777A4FC4B881EFA484D04397DE4312B7EE2BCEAD0ABAC0513D9C1819F59996AB5E94D48C098634D9907A4CA41B5A900F7CC299937B089A52C221333BF6B35242CA8D552A9CAA3BB18944D5CCEF69E752FA74A8BA3DD58159BAE375944B26A636457C30C451A58984BD6F028C8CBD3ED893ECD2F1536AFF3A97DB92C0E2FC93B9CC3837FDF56E4E3065D10B15F9A5A8925A5F772E4521CDC885180ACE49D98719BCE6770604C11E6DF090C199A5BBF1D695E1D55E2C9E135E89AB26446F6FED3FBD7DFE58EB75CA61B2D3DA4974908C7183E62FDD79C233387C78EDFF8AAC412EA36AD929808023EF04B8C9B7F980CC3FFD50E7356F51FBDC3DE8551C4F5B283D41E861E5604155E81748BF017268D4FBA8D253CA695300F5FB9A51190B24E2B17653519A3FE500B27DFF90A9FDF8404AFA8FAA1C4F738EB45969846E0A18693C06CA5FE2F2B5F7A33974A9453383E558317468B47DDAEE7A2AB90AA2446155CBF5B6473628C1D6BDA9044D4D96D1E3846AA7A65046C680BB173670FC04E5AC7D8CF41C6F233BD6945C0EA90958E9DBFE67D0EBCD";
	char testResult[512] = "9A5BD96DC0CD3B0089FBB6BE7D56BEE638431F5A";

	// Get a SHA1 hash instance
	CPPUNIT_ASSERT((hash = CryptoFactory::i()->getHashAlgorithm(HashAlgo::SHA1)) != NULL);

	ByteString b(testData);
	ByteString osslHash(testResult), shsmHash;

	// Now recreate the hash using our implementation in a single operation
	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	// Now recreate the hash in a multiple part operation
	shsmHash.wipe();

	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(0, 567)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567, 989)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567 + 989)));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	CryptoFactory::i()->recycleHashAlgorithm(hash);

	hash = NULL;
	rng = NULL;
}

void HashTests::testSHA224()
{
	char testData[4096] = "8F300FA699500CA5E1E22B38EC06B2EADCE7738CF6A55CB241577D53ED68BD017517C83AEB9CD3A25536FE999B7CD61D9E2BA85C39A7D27CE2CAAC8FE7C12FA1B2905DB85ABF230A813C1A72D2BD93BE0C5A79842FE35C5EF4B2E94524FB9594E3CCC63FE2A4E21D91C25DED832FE8BC9788697051C2042094585187394BD0AFD64FB627F3CCB350874FD0D454BF0254A72DD99ED211F68DC15B94A5682B73B89BF533746E8EEC5CCE45DF55466D7026A95A062EFD49FA270751A8D427095D38FB757FD4D1395B5DC7D302D9CF28C2E2D09796B9E0FC2D1F56BDE005FCD266B097822C170981DE2DB5E92582A612019823A3A062E228AB99AD6A0E47C1DEAC0078460252D83FA11253A3EFD374B2B392F0E27079F9FCACBB8EE6320C7A06327335AB34B757057E6C7FEC5004C4F675D2C4ACC6B359CC13D6F9FBE7E12EA7F6FF753CA8FB68059DB06D9804CBE08AAA55E0F20EFB7094695C1BF0D8E504EAD0836C0E6BB491E1691FDEB9ADFE0C92CED19A612DBCA01FF5A515E32B4EC06BDA00482DE721B6A85F00CAB264B34C5F3749D35603D0216C5AA426BBFF15637D1AE7805ACA572337232C27BDFC9D17D5D129242FD883E3C33250BA089EA03BD794C03D8DB9F57F87DC0A30E37F063B9C4C43518DC9F94E7BB18718D1DC79474CFAF60CBCD5CAA8AAA9FF573E26BB8B722C522ACF561FF34A0716FB7C1217050AD6E8323889B47D7551BC7554909651A9BAFCF5F0FEFD100D35B2E06D89C46E572E3EB5A07FEA702DF10BBCD2CAF2F2A62A9BC162F6369D0DBD9A6D6368ECDAFF4246DEE682C2B20EF4D7BBCCABBE0CD44CFBB531F2190E55C1A650B4D16DB2DA0453B23C4F056C1F791336A7266742976653B2E9377F500AAEFA1EA5FEE3469F4A85A484F69FBFFC1C163F78872AFB79231D9E5B4F8EFE2A9E8487BC33E4EC462F9E451BFEB306964D1F02873EEABCAB0F06361DD463398717537C7C255E3DEF1E9EC1567A600CFEB63F822A3C24B3524225A40F0CE948B0284AEF1F9BBE7618D18839725087DD6BFEE15653B981D6F57ED6685388F3B510140F437E93B92583609D4B873BBA3641B2DB56AB26D3DC27A85866D8F66C4A33D82932ECE277387E583BB20A689BE12B597C6199799A3A53B970596355A46F7F94B9D5E07B09B8C47355CE19D7D2EC1F1DA8F382F7C90ABABDED1D82C8BD673C008E16102025B8C8AD33832953B9E28A1C01353C513D55F4E38C0FBC1FE7C1FB6B93D10B464F851BFE26426BC09C599708CA1CA3413BC4B147CD69A761E688E3846949AC3F455CCDC0DDB4FDBC91D880B38C73B7AD4C8633598F4570E3BCBA3C5E785686E38D2926187CDF0479DF5B818EBDDFDA757C31DF009ED234B3C0F2BC1ADC700FD715AFEE46AE5CBAD02ED086912BE45BE2D61097F008E45067C4ADC4FDFE29220E6CF0C6F7F8";
	char testResult[512] = "A4E71A239CED53E936011DF80B516A07DFF13BB5B05BE43C46C05CCD";

	// Get a SHA224 hash instance
	CPPUNIT_ASSERT((hash = CryptoFactory::i()->getHashAlgorithm(HashAlgo::SHA224)) != NULL);

	ByteString b(testData);
	ByteString osslHash(testResult), shsmHash;

	// Now recreate the hash using our implementation in a single operation
	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	// Now recreate the hash in a multiple part operation
	shsmHash.wipe();

	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(0, 567)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567, 989)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567 + 989)));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	CryptoFactory::i()->recycleHashAlgorithm(hash);

	hash = NULL;
	rng = NULL;
}

void HashTests::testSHA256()
{
	char testData[4096] = "C75FF0C53CB345524BF9800FDF53CE448F3369F9CA08C817F8F07AEF0689274527F0AF1EE7FB5AE99B37456765F39880640BF6C5582111753758DF49A9A13B52379DA0C7AE930B03BC57CD819D2EBA0E38AD33D10876608B99A69D0A73F001065276055C4B659F62F77E7232B021BC085A2472293D2D1875028444902AB29D78C9919EC52B33C9F839B1F839C7FF39B68669C925F53AFC865C93D3694E2FD3F29FF0B7773BB412D0FDB1B506A4D501F840D01AE9BF18D7558109B10A6E55CD8CF9A5279D559810A0B08C84C2C8C294AFEC06C6219EB1647A51488BCDDAACE7C652BB0D3F1DC552E4B42BAFF52FD17239F4C08DDDA1BBA97D8D35D3A3DA32D66858B2D320A62AAA7C3909C3A34DCF9DAA69BEC13B7B3EAAEA8A56E2C6DB2507C43BE8990ADC4A281B8BFC6C76FAED50217F109594902FF7E0EDFF5C018B0FC76CACFF84FB9EA97D6D543EDA732089E82FDE3B9D087320014680E5EA2B21B534125806B650F3268DB7951460A9E32F2EEAAF69300E6F0B7F846ECBC2904E93E8D0423FBDCC3E34C5085A97636EA9961B00880BC381E37C3A6E3115256B1A9E1A915A401A8E507F97A09BF3881E852FEA614DF76D709DE6DC0FE54500E0C42132E9ACFFE4A4A98654190FC6C14FC3E5F42CDCCAE0DE05DEF5F6961D7D13DE15376F35142C5846013B7D4A8A6AB363EC7932415C556B83787C86491820AA29C2FE31E39A3D73D710C490D1AF863C4166C2301ECF6967506DCFF6F97CAE63245634153072FE27E9C4B5F90B3C42E4BDB4FB0E85A548CDE2D3B480ABFB1ED6F054017B120E41529909AC055A9F5A29E25D0E5D1A8E8550B9C85C300BBAAE030B22BB646B1FD3A90FA1C242A26071423B3E239EF65BE8B347663B6BF63436AFFD311D10FD24C8E8D05CA99A808C41E6C9C7B283FE5FE9E311642349D2799B6D0CACD5826CC6CC5DDA22FB8BBDD6C66BB722697E4558A75B6D24FDD55631C17FCB591CA3CBA180CEC241B0555A91DF4D47A648A7D901B02C1AB557C4E1CF33E277AFE008D586857203FD6CE695C8EDD446F74056B173D6A0B07A2A58B39366B58BB76AF96DF9C37A5A5E1F8419163CEAA304D451DC38B8F142C422FB475869667D55D88F7C2624FC0B29A958BB4A44C6B0439542AAB02319D46B7E482149DC9A7C7BEBE651C7E8404140601CDB3742F58B6EEDA137FA083BA9D297F91B41B34C3535230841F18B672ED0FD817B7E4E3CD1FC018CDA63C21C3C3ED68228DF85B8C26572D1B14EF10D5D0BAF6F3ED4B9A8C3DFEB4EDEA00CE2DB5903D510432BCBA791C1BBDD2770F2616AF855941E1BD710CEE4F17C4C7A9E283B12DE901A88D634E4ADE69ED529328D64F8069033A6B4DA5D8E0E1C0150DB862AAD54F2827B41CE1F21E3F890033FD46E438615DDC527D6D1260BDB79478D6A0BE2B58174648A2387E3AE";
	char testResult[512] = "3902802C215A5271439FE3E81AC7F21DA55545F71193A8DA8BEB0EAC8046A43B";

	// Get a SHA256 hash instance
	CPPUNIT_ASSERT((hash = CryptoFactory::i()->getHashAlgorithm(HashAlgo::SHA256)) != NULL);

	ByteString b(testData);
	ByteString osslHash(testResult), shsmHash;

	// Now recreate the hash using our implementation in a single operation
	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	// Now recreate the hash in a multiple part operation
	shsmHash.wipe();

	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(0, 567)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567, 989)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567 + 989)));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	CryptoFactory::i()->recycleHashAlgorithm(hash);

	hash = NULL;
	rng = NULL;
}

void HashTests::testSHA384()
{
	char testData[4096] = "11FCD8770F582C2D9C860D1EB4FDBFE40C551C6F3AF41A2540A39BA3A121BFEF995F1727A8703C29D0A44EB85B339F27955E1C679796A72980BB1D1BEF4D51A73174A983D36CDAD56BC1EEBA5DDD642094996531EEC94DE2FB6E0ED4F23BCB2F1D0B25505EB16306473DA17F809BD994FD9A737D29819BCCF94CA225D2EBB9D89DDA6E03E80A9DE87E44B72EFD397F9F344122B6591CED9BFDE8C0C42D8C17223A93F33B7D569291F91B619297F233C1454A168AAE126E89B0ED32B0B69AB095A1BB24BB7F1AAF8EE0BDBB43025EF3339F96C9EA352E78BA0661DE896E8F4DFAA7EF623F2B5305AD338448C5FCBDD6CC1F1222ED1D8F4C634B82591F8906DA8DDA9A0FFB0F1499B5BD08239F7EB3A02ACEC60FB76754D0AC5077C3B733D346F0BDE654CD612F60E2284115297A1557679901A911C2AC7323DA2FF3CD57895D7D181AD43AB7068609CE046B96B445EA08CDF50C40DFAFBA9F7082707A813B9C8E4E9D7F0230D5AFE40174693512DE96E3FAB1C8F6548880823645A0AD811694B293F788D0D523EBD81851594733EF45FC763B956044E29B29C195BDB317FBF97F41C601A2873C25557C3149F648424380FE79E4DC407A9CECD8F14B843C642FD9921F12786C8A1C6F8514C99672038693C5CF1FBE91F903E4ABC9E55B967B2F72FDF1A2EC09C14C94C001BAC47A0C36E9E6F34431381069CFD64D85F11285391A4DC7419B2EE8062F344538413E757EC258192B90F2CCC1186AB9A4ED5CE1290644CDEDAF03A4BF3E94B9F9D132ED159CE03586C5A69EF0A471146378BCCE799A3CD8D627B688BB28C9288F44D1218BC34A05BEAA398371ADC60CA8A2557AB69BE7B737F84BFFA93A1E1115F498600F52144E0D61055A2FC0CD45E20962CB2FF475896D733C74C2E95986389ED74B1497A35E73FEE0F36270CC65D76C1FE27A35E8F1FA0C5CA7F2C6003E21BD6677502CE268EB55B16DA863FA291AA111F338F10592AF86DFC297718365C04839748195E20A64BF42020846A46C94F1728549B8310A7FBCBA2C1441B033639CE52B6DBDA69F6ACC57F2DE6BAC57755734AFE4B77869C4D9B0DC56B115476A86A82F816CB148CDFD2B1DCCBEFCD4559B59AA88C6429F4A9EBB43B641144752A5E6F8BE1B739AA69FEBCB8D7439E5D917CA759982146E627FF81E80CEBC37BE0CF2A6C12A3E84A389FFD25013C491AE90A395D4DBCA81340E86848217AC426603CCAC981B5ED701CE9AB2851DC5F2ED72484FE99767C0FDB6F122B0C67926A637B57EC4F047804BC3A9BD55CBD78B83154BC27B6F8A66085EF02A206F329F3B1531ED657C75E29DF4276070AE5054F484A12990A9DD13C112FB6D3D57AE42E6A048870FDAABA48F73E03344C0E13832BAFCA2AF81AF19E5C28983F7BAAA5135803E78FBDDF7FB53C9B9B709274F05C774C77F0B84";
	char testResult[512] = "753659D55E4198325A3FED0A35761EC45038E8963B7525BF00D5A6A5D904DD9FA3FC80AB020185E08B14992ECC36A744";

	// Get a SHA384 hash instance
	CPPUNIT_ASSERT((hash = CryptoFactory::i()->getHashAlgorithm(HashAlgo::SHA384)) != NULL);

	ByteString b(testData);
	ByteString osslHash(testResult), shsmHash;

	// Now recreate the hash using our implementation in a single operation
	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	// Now recreate the hash in a multiple part operation
	shsmHash.wipe();

	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(0, 567)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567, 989)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567 + 989)));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	CryptoFactory::i()->recycleHashAlgorithm(hash);

	hash = NULL;
	rng = NULL;
}

void HashTests::testSHA512()
{
	char testData[4096] = "B9D5BD978E1B4A0268331AE4F0C4494EBA71125ED75B9B4244407271A29D93BF5BA0D6641EBF7A210A0905B5EBE675F7B900B89542D0DA6A3952DF9DC5430ECE31F4A5834CCF132724963A088DE43966DC9F46011F2AD458E8FCDBA18F6D183235687A6145401E9FA2EDF60F47F3835D953B5856B1F8308457D9CEC29246DC51F5B42BD5DE38BD89945667579C05756D1C47BB58C2C3D049C58C014C1F6D962BF5EBA1D273F5A7FD6D9291E293CD30B8D8B489E79E54A7A11953EAF4FB6AC7F99910ECE05B511E098B6AD33A599F5A8FA43692FDD9F2A26F316427A2ED1F231E328864CDF9BA2879E0A711F48038E653520C74B7EEA8858F17AC332B87EF4C70A838263C8B17B373161A5A15293D1A7991F0EEAAE0BB69C95DEC22A1CD6808122489703770B5E276735F9D149067A7F3E6A7D10A790253C81778BB79C06D1F98A2CA9C2E8C4BAC0A96F0F9736B85E7BB4F090C5BEB25F087AC037517A9599D94E072D03D9C48665DD17649E001E45966AB8F2C7D24E38AE4F0519F056B034E0ED2CA133F108105BE4823758543791225144AB0B9609F0F861B69EEA8707D6DB9046910F28C6F8E1D47F59DFE9B59F81242FCBE5A71669B90F7B4050E517CF6D3F2FB8759FB7300F12C786D36183FA64692F379D030D0EA90859D9D8E00F27500B41FD716E325E695E3D8EBB40A4E32AFA75F6F21013C8A3261E9C18291DC99142FFF91EAB6C2B55FF5DE03B1DCC88C79635B96F13B66CE6E26A1D5F6154F3C6E5F22AF4B0334FD4521AE57B6B13E2D3B777AF66742635740ED4FF5FC752B908A079A6BBE8499344096549E6FC6FEDBE57AAF2992E7CBB19931F43360415C39D65D2A92775C78DE97C712022460E48651EBDDB9E0527C7EE59F6C817499100C11E884633DC66EF85B176D7A529894CA8F364F559D97DC7CF1769813FB36F8F738E77354C14A560CB98DF92BB5BB16A5A3EDFB75CB78A718C070BE5D37A6F742F88B51BC633FA9E309311BCA908F4D5EE381FB593B7A53E4CE3A529160A2965D0C4CEDC6548D036A1A4FE4576DA9D35E0CDDE9567E792D5C77EFAC17E531F402E0BE99957A7D378E90C009F3231FEE6E3898B5D2C4291EAD396CA748AB0A39469B921BDC5F8066AB7F0D28EE0C502EBC95C2A39DB4241886736F17DE1F33BD384EFDEA951BF775A0D41A499E85D0488016D7A281A9F7C715EBB7CD6AFBA59C1A93A5AEA66D71C3058C5A293141EE4D2C3E65853EFBEFC55912F5C08B9990A66C313F3199C0769CDCA711E877766ECA773EF2FDBC0656B8427CA6442062D911B13F1C2E92D3F0C608B1D8F12914953720021D5648364F953F93CC49946E44FBAF154A2FE3A8055922F784FF9B56D5FAC35B65266A248794101C12750DB8C107D048E551E52EB3952CFA26345B4E21652F5F3B8F85DE04AF9731B75C79DDBD6FE1A5";
	char testResult[512] = "E1483A8525CE39705D14D60D8B19BD89087AED5FE6D8913AF8FC3F6F4EA2C1BB5957E205294B1EFAF20AE5EE39A9522F38B4514C3C15ED70BCBBD5821E385F95";

	// Get a SHA512 hash instance
	CPPUNIT_ASSERT((hash = CryptoFactory::i()->getHashAlgorithm(HashAlgo::SHA512)) != NULL);

	ByteString b(testData);
	ByteString osslHash(testResult), shsmHash;

	// Now recreate the hash using our implementation in a single operation
	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	// Now recreate the hash in a multiple part operation
	shsmHash.wipe();

	CPPUNIT_ASSERT(hash->hashInit());
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(0, 567)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567, 989)));
	CPPUNIT_ASSERT(hash->hashUpdate(b.substr(567 + 989)));
	CPPUNIT_ASSERT(hash->hashFinal(shsmHash));

	CPPUNIT_ASSERT(osslHash == shsmHash);

	CryptoFactory::i()->recycleHashAlgorithm(hash);

	hash = NULL;
	rng = NULL;
}
