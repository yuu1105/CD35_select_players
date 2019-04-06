#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <vector>
#include <algorithm>
#include <string>

// ファイル一行の最大文字数
#define MAX_LENGTH (256u)
// 1チームの最大コスト
#define MAX_COST (550u)
// 野手データ最大人数
#define MAX_BUTTER_NUM (100u)
// 投手データ最大人数
#define MAX_PITCHER_NUM (100u)
// 野手データファイル名
#define BUTTER_FILE_NAME ("Batter_Open.dat")
// 投手データファイル名
#define PITCHER_FILE_NAME ("Pitcher_Open.dat")

/*
 * 野手データ
 */
struct ButterData
{
    unsigned int id;             // ID
    std::string name;            // 氏名
    float daritsu;               // 打率
    // unsigned int shiai;          // 試合
    // unsigned int dasekisuu;      // 打席数
    // unsigned int dasuu;          // 打数
    // unsigned int tokuten;        // 得点
    // unsigned int anda;           // 安打
    // unsigned int ichiruida;      // 一塁打
    // unsigned int niruida;        // 二塁打
    // unsigned int sanruida;       // 三塁打
    // unsigned int honruida;       // 本塁打
    // unsigned int ruida;          // 塁打
    // unsigned int daten;          // 打点
    // unsigned int tourui;         // 盗塁
    // unsigned int touruisi;       // 盗塁死
    // unsigned int gida;           // 犠打
    // unsigned int gihi;           // 犠飛
    // unsigned int sikyu_4;        // 四球
    // unsigned int keien;          // 敬遠
    // unsigned int sikyu_dead;     // 死球
    // unsigned int sansin;         // 三振
    // unsigned int heisatsuda;     // 併殺打
    // float syutsuruiritsu;        // 出塁率
    // float tyoudaritsu;           // 長打率
    float ichiruidaritsu;        // 一塁打率
    float niruidaritsu;          // 二塁打率
    float sanruidaritsu;         // 三塁打率
    float honruidaritsu;         // 本塁打率
    float sikyuuritsu;           // 四球率
    float sanshinritsu;          // 三振率
    unsigned int cost;           // コスト

    bool operator<(const ButterData &another) const
    {
        return cost < another.cost;
    };
};

/*
 * 投手データ
 */
struct PitcherData
{
    unsigned int id;           // ID
    std::string name;          // 氏名
    float bougyoritsu;         // 防御率
    // unsigned int shiai;        // 試合
    // unsigned int syouri;       // 勝利
//    unsigned int haiboku;      // 敗北
//    unsigned int se_bu;        // セーブ
//    unsigned int ho_rudo;      // ホールド
//    unsigned int hp;           // ホールドポイント
//    unsigned int kantou;       // 完投
//    unsigned int kanpuu;       // 完封
//    unsigned int musikyuu;     // 無四球
//    float syouritsu;           // 勝率
//    unsigned int dasya;        // 打者
//    unsigned int toukyukai;    // 投球回
//    unsigned int hianda;       // 被安打
//    unsigned int hihonruida;   // 被本塁打
//    unsigned int yosikyu_4;    // 与四球
//    unsigned int keien;        // 敬遠
//    unsigned int yosikyu_dead; // 与死球
//    unsigned int datsusansin;  // 奪三振
//    unsigned int boutou;       // 暴投
//    unsigned int bo_ku;        // ボーク
//    unsigned int sitten;       // 失点
//    unsigned int jisekiten;    // 自責点
    float yoshikyuuritsu;        // 与四球率
    float datsusansinritsu;      // 奪三振率
    float hiandaritsu;           // 被安打率
    float hihonruidaritsu;       // 被本塁打率;
    unsigned int cost;         // コスト

    bool operator<(const ButterData &another) const
    {
        return cost < another.cost;
    };
};

class Main{
public :
	Main();
	virtual ~Main();

	bool readButterData();
	bool readPitcherData();
	bool selectPlayers();
	void displayTeam();
	void debugPrint();

private:
    bool isCostOver();
	// ファイルから読み込んだ野手データ
	std::vector<ButterData> m_butterData;
	// ファイルから読み込んだ投手データ
	std::vector<PitcherData> m_pitcherData;
	// 選択した野手データ
	std::vector<ButterData> m_selectedButterPlayers;
	// 選択した投手データ
	PitcherData m_seletedPitcherPlayer;

};

Main::Main()
: m_butterData()
, m_pitcherData()
, m_selectedButterPlayers()
, m_seletedPitcherPlayer()
{
}

Main::~Main(){}


/*
 * 選手を選ぶ(改造必須)
 */
bool
Main::selectPlayers()
{
    // 例として、コストが低い選手を野手9人、投手１人を選ぶ

    // 野手
    {
    	std::sort(m_butterData.begin(), m_butterData.end());
    	std::copy(m_butterData.begin(), m_butterData.begin()+9, std::back_inserter(m_selectedButterPlayers) );
    }

    // 投手
    {
        m_seletedPitcherPlayer.cost = UINT_MAX;
        for( unsigned int i = 0 ; i < m_pitcherData.size() ; i++)
        {
            if( m_seletedPitcherPlayer.cost > m_pitcherData[i].cost )
            {
                m_seletedPitcherPlayer = m_pitcherData[i];
            }
        }
    }

    if( isCostOver() )
    {
        fprintf(stderr, "cost over\n");
        return false;
    }

    return true;
}

/*
 * コストオーバーチェック
 */
bool
Main::isCostOver()
{
    unsigned int sum = 0u;
    
    unsigned int i;
    for( i = 0u ; i < 9 ; i++ )
    {
        sum += m_selectedButterPlayers[i].cost;
    }
    
    sum += m_seletedPitcherPlayer.cost;
    
    if( sum > MAX_COST )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 * 処理結果出力
 */
void
Main::displayTeam()
{
    printf("Example Team\n");
    unsigned int i;
    for( i = 0u ; i < 9 ; i++ )
    {
        printf("%d#%s\n",
            m_selectedButterPlayers[i].id,
            m_selectedButterPlayers[i].name.c_str());
    }
    printf("%d#%s\n",
        m_seletedPitcherPlayer.id,
        m_seletedPitcherPlayer.name.c_str() );
}

/*
 * 文字列->UINT変換
 */
bool
analysisDataUInt(char* str, unsigned int& value)
{
    if( NULL == str )
    {
        fprintf(stderr, "analysis error %d\n", __LINE__);
        return false;
    }

    char *e;
    value = (unsigned int)strtoul(str, &e, 10);

    if (*e != '\0') {
		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	fprintf(stderr, "%d:%ld文字目の\'%c\',%dが変換不可\n", __LINE__, e-str+1, *e, *e);
    	return false;
    }

    return true;
}

/*
 * 文字列->Float変換
 */
int
analysisDataFloat(char* str, float& value)
{
    if( NULL == str )
    {
        fprintf(stderr, "analysis error %d\n", __LINE__);
        return false;
    }

    char *e;
    value = (float)strtod(str, &e);

    if (*e != '\0') {
		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	fprintf(stderr, "%d:%ld文字目の\'%c\',%dが変換不可\n", __LINE__, e-str+1, *e, *e);
    	return false;
    }

    return true;
}


/*
 * 野手データ読み込み
 */
bool
Main::readButterData()
{
    FILE* fp = fopen(BUTTER_FILE_NAME, "r");
    if( NULL == fp )
    {
        // 読み込み失敗
        fprintf(stderr, "fopen err %s\n", BUTTER_FILE_NAME);
        return false;
    }

    // 読み込みバッファ
    char buffer[MAX_LENGTH];
    for( unsigned int i = 0u ; i < MAX_BUTTER_NUM ; i++)
    {
        // 一行読み込み
        memset(buffer, 0, sizeof(buffer));
        if (NULL == fgets(buffer, sizeof(buffer)-1, fp)) {
            break;
        }

        if( '\r' == buffer[strlen(buffer) - 2]  )
        {
            buffer[strlen(buffer) - 2] = '\0';
        }
        else if('\n' == buffer[strlen(buffer) - 1] )
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        ButterData data;
        // ID
        if(!analysisDataUInt(strtok(buffer, ","), data.id))
        {
            continue;
        }

        // 名前
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }
            data.name = tok;
        }

        // 打率
        if(!analysisDataFloat(strtok( NULL, ","), data.daritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#if 0
         // 試合
        if(!analysisDataUInt(strtok( NULL, ","), data.shiai))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打席数
        if(!analysisDataUInt(strtok( NULL, ","), data.dasekisuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打数
        if(!analysisDataUInt(strtok( NULL, ","), data.dasuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 得点
        if(!analysisDataUInt(strtok( NULL, ","), data.tokuten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 安打
        if(!analysisDataUInt(strtok( NULL, ","), data.anda))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 二塁打
        if(!analysisDataUInt(strtok( NULL, ","), data.niruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 三塁打
        if(!analysisDataUInt(strtok( NULL, ","), data.sanruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 本塁打
        if(!analysisDataUInt(strtok( NULL, ","), data.honruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 塁打
        if(!analysisDataUInt(strtok( NULL, ","), data.ruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打点
        if(!analysisDataUInt(strtok( NULL, ","), data.daten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 盗塁
        if(!analysisDataUInt(strtok( NULL, ","), data.tourui))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 盗塁死
        if(!analysisDataUInt(strtok( NULL, ","), data.touruisi))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 犠打
        if(!analysisDataUInt(strtok( NULL, ","), data.gida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 犠飛
        if(!analysisDataUInt(strtok( NULL, ","), data.gihi))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 四球
        if(!analysisDataUInt(strtok( NULL, ","), data.sikyu_4))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 敬遠
        if(!analysisDataUInt(strtok( NULL, ","), data.keien))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 死球
        if(!analysisDataUInt(strtok( NULL, ","), data.sikyu_dead))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 三振
        if(!analysisDataUInt(strtok( NULL, ","), data.sansin))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 併殺打
        if(!analysisDataUInt(strtok( NULL, ","), data.heisatsuda))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 出塁率
        if(!analysisDataFloat(strtok( NULL, ","), data.syutsuruiritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 長打率
        if(!analysisDataFloat(strtok( NULL, ","), data.tyoudaritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#endif
        // 一塁打率
		if(!analysisDataFloat(strtok( NULL, ","), data.ichiruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 二塁打率
		if(!analysisDataFloat(strtok( NULL, ","), data.niruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 三塁打率
		if(!analysisDataFloat(strtok( NULL, ","), data.sanruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 本塁打率
		if(!analysisDataFloat(strtok( NULL, ","), data.honruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 四球率
		if(!analysisDataFloat(strtok( NULL, ","), data.sikyuuritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 三振率
		if(!analysisDataFloat(strtok( NULL, ","), data.sanshinritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // コスト
        if(!analysisDataUInt(strtok(NULL, ","), data.cost))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        m_butterData.push_back(data);

    }

    fclose(fp);

    return true;
}

/*
 * 投手データ読み込み
 */
bool
Main::readPitcherData()
{
    FILE* fp = fopen(PITCHER_FILE_NAME, "r");
    if( NULL == fp )
    {
        // 読み込み失敗
        fprintf(stderr, "fopen err %s\n", PITCHER_FILE_NAME);
        return false;
    }

    // 読み込みバッファ
    char buffer[MAX_LENGTH];

    unsigned int i;
    for( i = 0u ; i < MAX_PITCHER_NUM ; i++)
    {
        // 一行読み込み
        memset(buffer, 0, sizeof(buffer));
        if (NULL == fgets(buffer, sizeof(buffer)-1, fp)) {
            break;
        }

        if( '\r' == buffer[strlen(buffer) - 2]  )
        {
            buffer[strlen(buffer) - 2] = '\0';
        }
        else if('\n' == buffer[strlen(buffer) - 1] )
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        PitcherData data;
        // ID
        {
            char* tok = strtok(buffer, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }

            char *e;
            data.id = (unsigned int)strtoul(tok, &e, 10);

    	    if (*e != '\0') {
        		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\'が変換不可\n", __LINE__, e-tok+1, *e);
    	    	continue;
    	    }
        }

        // 名前
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }
            data.name = tok;
        }

        // 防御率
        if(!analysisDataFloat(strtok( NULL, ","), data.bougyoritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#if 0
        // 試合
        if(!analysisDataUInt(strtok( NULL, ","), data.shiai))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 勝利
        if(!analysisDataUInt(strtok( NULL, ","), data.syouri))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 敗北
        if(!analysisDataUInt(strtok( NULL, ","), data.haiboku))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // セーブ
        if(!analysisDataUInt(strtok( NULL, ","), data.se_bu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // ホールド
        if(!analysisDataUInt(strtok( NULL, ","), data.ho_rudo))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // ホールドポイント
        if(!analysisDataUInt(strtok( NULL, ","), data.hp))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 完投
        if(!analysisDataUInt(strtok( NULL, ","), data.kantou))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 完封
        if(!analysisDataUInt(strtok( NULL, ","), data.kanpuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 無四球
        if(!analysisDataUInt(strtok( NULL, ","), data.musikyuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 勝率
        if(!analysisDataFloat(strtok( NULL, ","), data.syouritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打者
        if(!analysisDataUInt(strtok( NULL, ","), data.dasya))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 投球回
        float tmp;
        if(!analysisDataFloat(strtok( NULL, ","), tmp))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        data.toukyukai = (int)tmp;

        // 被安打
        if(!analysisDataUInt(strtok( NULL, ","), data.hianda))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 被本塁打
        if(!analysisDataUInt(strtok( NULL, ","), data.hihonruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 与四球
        if(!analysisDataUInt(strtok( NULL, ","), data.yosikyu_4))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 敬遠
        if(!analysisDataUInt(strtok( NULL, ","), data.keien))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 与死球
        if(!analysisDataUInt(strtok( NULL, ","), data.yosikyu_dead))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 奪三振
        if(!analysisDataUInt(strtok( NULL, ","), data.datsusansin))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 暴投
        if(!analysisDataUInt(strtok( NULL, ","), data.boutou))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // ボーク
        if(!analysisDataUInt(strtok( NULL, ","), data.bo_ku))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 失点
        if(!analysisDataUInt(strtok( NULL, ","), data.sitten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 自責点
        if(!analysisDataUInt(strtok( NULL, ","), data.jisekiten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#endif
        // 与四球率
        if(!analysisDataFloat(strtok( NULL, ","), data.yoshikyuuritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // 奪三振率
        if(!analysisDataFloat(strtok( NULL, ","), data.datsusansinritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // 被安打率
        if(!analysisDataFloat(strtok( NULL, ","), data.hiandaritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // 被本塁打率
        if(!analysisDataFloat(strtok( NULL, ","), data.hihonruidaritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // コスト
        if(!analysisDataUInt(strtok(NULL, ","), data.cost))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        m_pitcherData.push_back(data);

    }

    fclose(fp);

    return true;
}


int main(void)
{
	Main obj;

    //printf("#野手データ読み込み\n");
    if(!obj.readButterData())
    {
        return 1;
    }

    // printf("#投手データ読み込み\n");
    if(!obj.readPitcherData())
    {
        return 1;
    }
    // obj.debugPrint();

    if(!obj.selectPlayers())
    {
        return 1;
    }
    printf("#表示\n");
    obj.displayTeam();

    return 0;
}

void Main::debugPrint()
{
    printf("#######BUTTER\n");
    for( unsigned int i = 0u ; i < m_butterData.size() ; i++ )
    {
        printf("%d,%s,%f,%f,%f,%f,%f,%f,%f,%d\n",
            m_butterData[i].id,
            m_butterData[i].name.c_str(),
            m_butterData[i].daritsu,
            m_butterData[i].ichiruidaritsu,
            m_butterData[i].niruidaritsu,
            m_butterData[i].sanruidaritsu,
            m_butterData[i].honruidaritsu,
            m_butterData[i].sikyuuritsu,
            m_butterData[i].sanshinritsu,
            m_butterData[i].cost);
    }

    printf("#######PITCHER\n");
    for( unsigned int i = 0u ; i < m_pitcherData.size() ; i++ )
    {
        printf("%d,%s,%f,%f,%f,%f,%f,%d\n",
            m_pitcherData[i].id,
            m_pitcherData[i].name.c_str(),
            m_pitcherData[i].bougyoritsu,
            m_pitcherData[i].yoshikyuuritsu,
            m_pitcherData[i].datsusansinritsu,
            m_pitcherData[i].hiandaritsu,
            m_pitcherData[i].hihonruidaritsu,
            m_pitcherData[i].cost);
    }
}
