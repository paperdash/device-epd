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
        target: 'http://paperdash-epd.local:80',
        ws: true,
        changeOrigin: true,
      },
    },
  },
  chainWebpack: (config) => {
    const svgRule = config.module.rule('svg')

    svgRule.uses.clear()

    svgRule
      .use('babel-loader')
      .loader('babel-loader')
      .end()
      .use('vue-svg-loader')
      .loader('vue-svg-loader')
      .options({
        svgo: {
          plugins: [
            { cleanupIDs: false },
            { collapseGroups: false },
            { removeEmptyContainers: false },
          ],
        },
      })
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
          appVersionJson,
        ],
      }
    } else {
      // mutate for development...
      return {
        plugins: [
          // add version info
          appVersionJson,
        ],
      }
    }
  },
}
