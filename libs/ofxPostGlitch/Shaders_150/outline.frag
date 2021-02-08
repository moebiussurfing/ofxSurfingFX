/*
	MAX-MINフィルタという手法を使ったエッジ検出の例です。
	周辺ピクセルの最大値と最小値を取り、その差を輝度として出力しています。
*/

#version 150

uniform sampler2DRect tex0;

in vec2 varyingtexcoord;
out vec4 outputColor;


void main (void)
{
	int step = 3;
	vec4 cols[9];/*自分の周辺のピクセルの色情報*/
    vec2 texCoord = vec2(varyingtexcoord.x, varyingtexcoord.y);/*自分の色情報*/

    /*周辺の色情報をピックアップして、RGBの平均値(グレースケール)をR要素に代入する*/
	for (int i = 0;i < step;i++){
		for (int j = 0;j < step;j++){
			cols[i*step+j] = texture(tex0, vec2((varyingtexcoord.x+float(j)-2.0), (varyingtexcoord.y+float(i)-2.0)));
			cols[i*step+j].r = (cols[i*step+j].r + cols[i*step+j].g + cols[i*step+j].b) / 3.0;
		}
	}

	/*取得した色の中で、輝度の最大値と最小値をピックアップする*/
	float mn = 1.0, mx = 0.0;
	for (int i = 0;i < step*step;i++){
		mn = min(cols[i].r,mn);
		mx = max(cols[i].r,mx);
	}

	/*最大・最小の差を計算して出力する*/
	float dst = abs(mx-mn);
    outputColor = vec4(dst,dst,dst,1.0);
}
