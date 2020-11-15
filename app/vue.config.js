const webpack = require('webpack')
const CompressionPlugin = require('compression-webpack-plugin')
const VersionFile = require('webpack-version-file')

// get git info from command line
const commitHash = require('child_process')
  .execSync('git rev-parse HEAD')
  .toString()

const buildInfo = {
  commitHash: commitHash.trim(),
  buildTime: JSON.stringify(new Date().getTime() / 1000 | 0),
}

/*
const appVersionVue = new webpack.DefinePlugin({
  __COMMIT_HASH__: buildInfo.commitHash,
  __BUILD_TIME__: buildInfo.buildTime,
})
*/

const appVersionJson = new VersionFile({
  output: '../data/dist/version.json',
  template: './version.ejs',
  data: {
    commitHash: buildInfo.commitHash,
    buildTime: buildInfo.buildTime,
  },
})

module.exports = {
  outputDir: '../data/dist',
  filenameHashing: false,
  productionSourceMap: false,
  transpileDependencies: [
    'vuetify',
  ],
  devServer: {
    proxy: {
      '^/': {
        target: 'http://paperdash-display:80',
        ws: true,
        changeOrigin: true,
      },
    },
  },
  configureWebpack: config => {
    if (process.env.NODE_ENV === 'production') {
      // optimize build for esp32
      return {
        plugins: [
          // reduce total size of the app
          new webpack.optimize.LimitChunkCountPlugin({
            maxChunks: 1,
          }),
          // use only pre compressed files
          new CompressionPlugin({
            deleteOriginalAssets: true,
          }),
          // add version info
          // appVersionVue,
          appVersionJson,
        ],
      }
    } else {
      // mutate for development...
      return {
        plugins: [
          // add version info
          // appVersionVue,
          appVersionJson,
        ],
      }
    }
  },
}
