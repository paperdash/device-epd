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
				target: 'http://192.168.178.65:80',
				ws: true,
				changeOrigin: true
			},
		}
	}
}