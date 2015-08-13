package Table;

public class Traffic_data {	
	private String _T_DATE;
	private String _T_TIME;
	private String _CellID;
	private String _nTCH;
	private String _traff;
	private String _rate;
	private String _thtraff;
	private String _callnum;
	private String _congsnum;
	private String _callcong;
	
	private int T_DATE;
	private int T_TIME;
	private int CellID;
	private int nTCH;
	private float traff;
	private float rate;
	private float thtraff;
	private int callnum;
	private int congsnum;
	private float callcong;

	public Traffic_data(String _T_DATE, String _T_TIME, String _CellID, String _nTCH, 
			String _traff, String _rate, String _thtraff, String _callnum
			, String _congsnum, String _callcong){
		this._T_DATE = _T_DATE;
		this._T_TIME = _T_TIME;
		this._CellID = _CellID;
		this._nTCH = _nTCH;
		this._traff = _traff;
		this._rate = _rate;
		this._thtraff = _thtraff;
		this._callnum = _callnum;
		this._congsnum = _congsnum;
		this._callcong = _callcong;
		
		this.T_DATE = 0;
		this.T_TIME = 0;
		this.CellID = 0;
		this.nTCH = 0;
		this.traff = 0;
		this.rate = 0;
		this.thtraff = 0;
		this.callnum = 0;
		this.congsnum = 0;
		this.callcong = 0;
	}	

	public boolean checkRow(){
		try{
			CellID = Integer.parseInt(_CellID);
			T_DATE = Integer.parseInt(_T_DATE);
			T_TIME = Integer.parseInt(_T_TIME);
			if (CellID < 0 || T_DATE < 0 || T_TIME < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}

	public int getT_DATE() {
		return T_DATE;
	}

	public int getT_TIME() {
		return T_TIME;
	}

	public int getCellID() {
		return CellID;
	}

	public int getnTCH() {
		return nTCH;
	}

	public float getTraff() {
		return traff;
	}

	public float getRate() {
		return rate;
	}

	public float getThtraff() {
		return thtraff;
	}

	public int getCallnum() {
		return callnum;
	}

	public int getCongsnum() {
		return congsnum;
	}

	public float getCallcong() {
		return callcong;
	}
}
