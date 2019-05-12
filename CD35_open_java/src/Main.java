import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Main {
    /** ファイル一行の最大文字数 */
    private final static Integer MAX_LENGTH = 256;
    /** 1チームの最大コスト */
    private final static Integer MAX_COST = 550;
    /** 野手データ最大人数 */
    private final static Integer MAX_BUTTER_NUM = 100;
    /** 投手データ最大人数 */
    private final static Integer MAX_PITCHER_NUM = 100;
    /** 野手データファイル名 */
    private final static String BUTTER_FILE_NAME = "Batter_Open.dat";
    /** 投手データファイル名 */
    private final static String PITCHER_FILE_NAME = "Pitcher_Open.dat";

    /**
     * 野手データ
     */
    private static class ButterData
    {
        public ButterData(String dataStr)
        {
            String[] datas = dataStr.split(",", 0);
            id = Integer.parseInt(datas[0]);
            name = datas[1];
            daritsu = Float.parseFloat(datas[2]);
            ichiruidaritsu = Float.parseFloat(datas[3]);
            niruidaritsu =  Float.parseFloat(datas[4]);
            sanruidaritsu =  Float.parseFloat(datas[5]);
            honruidaritsu =  Float.parseFloat(datas[6]);
            sikyuuritsu =  Float.parseFloat(datas[7]);
            sanshinritsu =  Float.parseFloat(datas[8]);
            cost = Integer.parseInt(datas[9]);

        }
        public Integer id;             // ID
        public String name;            // 選手名
        public Float daritsu;          // 打率
        public Float ichiruidaritsu;   // 一塁打率
        public Float niruidaritsu;     // 二塁打率
        public Float sanruidaritsu;    // 三塁打率
        public Float honruidaritsu;    // 本塁打率
        public Float sikyuuritsu;      // 四球率
        public Float sanshinritsu;     // 三振率
        public Integer cost;           // コスト
    }

    /**
     * 投手データ
     */
    private static class PitcherData
    {
        public PitcherData(String dataStr)
        {
            String[] datas = dataStr.split(",", 0);
            id = Integer.parseInt(datas[0]);
            name = datas[1];
            bougyoritsu = Float.parseFloat(datas[2]);
            yoshikyuuritsu = Float.parseFloat(datas[3]);;
            datsusansinritsu = Float.parseFloat(datas[4]);;
            hiandaritsu = Float.parseFloat(datas[5]);;
            hihonruidaritsu = Float.parseFloat(datas[6]);;
            cost = Integer.parseInt(datas[7]);
        }
        public Integer id;           // ID
        public String name;          // 氏名
        public Float bougyoritsu;    // 防御率
        public Float yoshikyuuritsu;   // 与四球率
        public Float datsusansinritsu; // 奪三振率
        public Float hiandaritsu;      // 被安打率
        public Float hihonruidaritsu;  // 被本塁打率;
        public Integer cost;         // コスト
    }

    /** ファイルから読み込んだ野手データ */
    ArrayList<ButterData> butterData = new ArrayList<ButterData>();
    /** ファイルから読み込んだ投手データ */
    ArrayList<PitcherData> pitcherData = new ArrayList<PitcherData>();
    /** 選択した野手データ */
    List<ButterData> selectedButterPlayers;
    /** 選択した投手データ */
    PitcherData selectedPitcherPlayer;

    /**
     * 選手を選ぶ(改造必須)
     */
    private void selectPlayers()
    {
        // 例として、コストが低い選手を野手9人、投手１人を選ぶ

        // 野手
        {
            Collections.sort(butterData, new Comparator<ButterData>(){
                public int compare(ButterData o1, ButterData o2) {
                    return o1.cost.compareTo(o2.cost);
                }
            });
            selectedButterPlayers = butterData.subList(0,9);
        }

        // 投手
        {
            Integer minCost = Integer.MAX_VALUE;
            for( PitcherData d : pitcherData )
            {
                if( minCost > d.cost )
                {
                    minCost = d.cost;
                    selectedPitcherPlayer = d;
                }
            }

        }

        if( isCostOver() )
        {
            System.err.println("cost over");
            return;
        }

        return;
    }

    /*
     * コストオーバーチェック
     */
    public Boolean isCostOver()
    {
        int sum = 0;

        for( int i = 0 ; i < 9 ; i++ )
        {
            sum += selectedButterPlayers.get(i).cost;
        }

        sum += selectedPitcherPlayer.cost;

        if( sum > MAX_COST )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * 処理結果出力
     */
    private void displayTeam()
    {
        System.out.println("Example Team");
        for( ButterData d : selectedButterPlayers )
        {
            System.out.println("" + d.id + "#" + d.name );
        }
        System.out.println("" + selectedPitcherPlayer.id + "#" + selectedPitcherPlayer.name );
    }


    /**
     * 野手データ読み込み
     */
    private void readButterData()
    {
        try{
            BufferedReader in = new BufferedReader(new FileReader(BUTTER_FILE_NAME));

            for( int i = 0 ; i < MAX_BUTTER_NUM ; i++ )
            {
                String line = in.readLine();
                if( line == null ) break;

                butterData.add( new ButterData(line) );
            }

           in.close();

        }catch(FileNotFoundException e){
            System.out.println(e);

        }catch(IOException e){
            System.out.println(e);
        }
    }

    /**
     * 投手データ読み込み
     */
    private void readPitcherData()
    {
        try{
            BufferedReader in = new BufferedReader(new FileReader(PITCHER_FILE_NAME));

            for( int i = 0 ; i < MAX_PITCHER_NUM ; i++ )
            {
                String line = in.readLine();
                if( line == null ) break;

                pitcherData.add( new PitcherData(line) );
            }

            in.close();

        }catch(FileNotFoundException e){
            System.out.println(e);

        }catch(IOException e){
            System.out.println(e);
        }
    }


    public static void main(String[] args) throws Exception {

        Main obj = new Main();

        obj.readButterData();

        obj.readPitcherData();

        // obj.debugPrint();

        obj.selectPlayers();

        obj.displayTeam();

    }

    private void debugPrint()
    {
        System.out.printf("#######BUTTER\n");

        for( ButterData d : butterData )
        {
            System.out.printf("%d,%s,%f,%f,%f,%f,%f,%f,%f,%d\n",
                    d.id,
                    d.name,
                    d.daritsu,
                    d.ichiruidaritsu,
                    d.niruidaritsu,
                    d.sanruidaritsu,
                    d.honruidaritsu,
                    d.sikyuuritsu,
                    d.sanshinritsu,
                    d.cost);
        }

        System.out.printf("#######PITCHER\n");
        for( PitcherData d : pitcherData )
        {
            System.out.printf("%d,%s,%f,%f,%f,%f,%f,%d\n",
                    d.id,
                    d.name,
                    d.bougyoritsu,
                    d.yoshikyuuritsu,
                    d.datsusansinritsu,
                    d.hiandaritsu,
                    d.hihonruidaritsu,
                    d.cost);
        }
    }

}
