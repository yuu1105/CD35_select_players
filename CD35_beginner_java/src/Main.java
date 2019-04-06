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
    private final static Integer MAX_COST = 100;
    /** 野手データ最大人数 */
    private final static Integer MAX_BUTTER_NUM = 100;
    /** 投手データ最大人数 */
    private final static Integer MAX_PITCHER_NUM = 100;
    /** 野手データファイル名 */
    private final static String BUTTER_FILE_NAME = "File1";
    /** 投手データファイル名 */
    private final static String PITCHER_FILE_NAME = "File2";

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
            cost = Integer.parseInt(datas[3]);

        }
        public Integer id;    // ID
        public String name;   // 氏名
        public Float daritsu; // 打率
        public Integer cost;  // コスト
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
            cost = Integer.parseInt(datas[3]);
        }
        public Integer id;        // ID
        public String name;       // 氏名
        public Float bougyoritsu; // 防御率
        public Integer cost;      // コスト
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
            selectedButterPlayers = butterData.subList(0,8);
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

        return;
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
            System.out.printf("%d,%s,%f,%d\n",
                d.id,
                d.name,
                d.daritsu,
                d.cost);
        }

        System.out.printf("#######PITCHER\n");
        for( PitcherData d : pitcherData )
        {
            System.out.printf("%d,%s,%f,%d\n",
                d.id,
                d.name,
                d.bougyoritsu,
                d.cost);
        }
    }

}
