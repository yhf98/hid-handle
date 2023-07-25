<template>
    <div class="panel">
      <!--显示-->
      <div class="setting-row">
        <span>{{ this.$t("lang.index.itemDialog.visible") }}</span>
        &NonBreakingSpace;&NonBreakingSpace;&NonBreakingSpace;&NonBreakingSpace;
        <el-switch
          v-model="selectItemData.isShow"
          active-text=""
          @change="changeHandle(selectItemData.isShow, 'isShow')"
        >
        </el-switch>
      </div>
      <!--元素名称-->
      <div class="setting-row">
        <el-input
          type="text"
          v-model="selectItemData.name"
          @change="changeHandle(selectItemData.name, 'name')"
        >
          <span slot="prepend"
            >{{ this.$t("lang.index.itemDialog.name") }}：</span
          >
        </el-input>
      </div>
      <!--数据-->
      <div class="setting-row">
        <span style="width: 60px">{{ this.$t("lang.index.itemDialog.data") }}：</span>
        <el-cascader
          filterable
          :options="dataOptions"
          @change="tagTypeChange"
          v-model="selectItemData.cas_data"
        ></el-cascader>
      </div>
      <!--背景颜色-->
      <div class="setting-row">
        <el-input
          type="color"
          v-model="selectItemData.bgColor"
          @change="changeHandle(selectItemData.bgColor, 'bgColor')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.bgColor") }}：</span
          >
          <span slot="append">RGB</span>
        </el-input>
      </div>
      <!--透明度-->
      <div class="setting-row">
        <el-input
          type="number"
          min="0"
          max="255"
          v-model="selectItemData.opa"
          @change="changeHandle(selectItemData.opa, 'opa')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.opacity") }}：</span
          >
        </el-input>
      </div>
      <!--X坐标-->
      <div class="setting-row">
        <el-input
          type="number"
          v-model="selectItemData.x"
          @change="changeHandle(selectItemData.x, 'x')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.xPoint") }}：</span
          >
          <span slot="append">px</span>
        </el-input>
      </div>
      <!--Y坐标-->
      <div class="setting-row">
        <el-input
          type="number"
          v-model="selectItemData.y"
          @change="changeHandle(selectItemData.y, 'y')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.yPoint") }}：</span
          >
          <span slot="append">px</span>
        </el-input>
      </div>
      <!--宽度-->
      <div class="setting-row">
        <el-input
          type="number"
          v-model="selectItemData.w"
          @change="changeHandle(selectItemData.w, 'w')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.width") }}：</span
          >
          <span slot="append">px</span>
        </el-input>
      </div>
      <!--高度-->
      <div class="setting-row">
        <el-input
          type="number"
          v-model="selectItemData.h"
          @change="changeHandle(selectItemData.h, 'h')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.height") }}：</span
          >
          <span slot="append">px</span>
        </el-input>
      </div>
      <!--字体颜色-->
      <div class="setting-row">
        <el-input
          type="color"
          v-model="selectItemData.color"
          @change="changeHandle(selectItemData.color, 'color')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.fontColor") }}：</span
          >
          <span slot="append">RGB</span>
        </el-input>
      </div>
      <!--字体大小-->
      <div class="setting-row">
        <el-input
          type="number"
          v-model="selectItemData.fontSize"
          @change="changeHandle(selectItemData.fontSize, 'fontSize')"
        >
          <span class="setting-title" slot="prepend"
            >{{ this.$t("lang.index.itemDialog.fontSize") }}：</span
          >
          <span slot="append">px</span>
        </el-input>
      </div>
      <!--单位【同数据一同显示】-->
      <div class="setting-row" v-if="selectItemData.type == 0">
        <span style="width: 60px">{{ this.$t("lang.index.itemDialog.unit") }}：</span>
        <el-select v-model="selectItemData.unit" placeholder="">
          <el-option :key="0" label="K" value="byte2kb" />
          <el-option :key="1" label="M" value="byte2mb" />
          <el-option :key="2" label="G" value="byte2gb" />
        </el-select>
      </div>
      
    </div>
  </template>