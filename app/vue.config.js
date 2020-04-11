const webpack = require('webpack');

module.exports = {
	"outputDir": "../data/dist",
	"filenameHashing": false,
	"productionSourceMap": false,
	"transpileDependencies": [
		"vuetify"
	],
	devServer: {
		proxy: {
			'^/': {
				target: 'http://192.168.178.62:80',
				ws: true,
				changeOrigin: true
			},
		}
	},
	configureWebpack: config => {
		if (process.env.NODE_ENV === 'production') {
			// optimize build for esp32
			return {
				plugins: [
					new webpack.optimize.LimitChunkCountPlugin({
						maxChunks: 1
					})
				]
			}
		} else {
			// mutate for development...
		}
	}
}