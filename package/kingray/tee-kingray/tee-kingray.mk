################################################################################
#
# Kingray tee-kingray For Linux
#
################################################################################

TEE_KINGRAY_VERSION = develop
TEE_KINGRAY_SITE = $(TOPDIR)/../external/security
TEE_KINGRAY_SITE_METHOD = local

TEE_KINGRAY_LICENSE = GPL-2.0
TEE_KINGRAY_LICENSE_FILES = rk_tee_user/v2/LICENSE.md

TEE_KINGRAY_OUTPUT=$(@D)/rk_tee_user/${BR2_PACKAGE_TEE_KINGRAY_TEE_VERSION}/out
TEE_KINGRAY_PRE_BIN=$(@D)/bin/optee_${BR2_PACKAGE_TEE_KINGRAY_TEE_VERSION}

ifeq ($(BR2_ARCH_IS_64),y)
TEE_KINGRAY_TOOLCHAIN_64 = $(TARGET_CROSS)
TEE_KINGRAY_TOOLCHAIN_32 = $(BR2_PACKAGE_TEE_KINGRAY_EXTRA_TOOLCHAIN)
TEE_KINGRAY_ARCH=64
else
TEE_KINGRAY_TOOLCHAIN_64 = $(BR2_PACKAGE_TEE_KINGRAY_EXTRA_TOOLCHAIN)
TEE_KINGRAY_TOOLCHAIN_32 = $(TARGET_CROSS)
TEE_KINGRAY_ARCH=
endif

define TEE_KINGRAY_BUILD_CMDS
	cp $(TOPDIR)/package/kingray/tee-kingray/kr_app/host \
		$(@D)/rk_tee_user/${BR2_PACKAGE_TEE_KINGRAY_TEE_VERSION}/host/kr_app -r ; \
	cp $(TOPDIR)/package/kingray/tee-kingray/kr_app/ta \
		$(@D)/rk_tee_user/${BR2_PACKAGE_TEE_KINGRAY_TEE_VERSION}/ta/kr_app -r

	cd $(@D)/rk_tee_user/$(BR2_PACKAGE_TEE_KINGRAY_TEE_VERSION) ; \
	AARCH64_TOOLCHAIN=$(TEE_KINGRAY_TOOLCHAIN_64) ARM32_TOOLCHAIN=$(TEE_KINGRAY_TOOLCHAIN_32) \
			  ./build.sh $(BR2_PACKAGE_TEE_KINGRAY_COMPILE_CMD)
endef

define TEE_KINGRAY_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 755 $(TEE_KINGRAY_OUTPUT)/kr_app/anti_copy_board $(TARGET_DIR)/usr/bin/
	mkdir $(TARGET_DIR)/lib/optee_armtz || true
	$(INSTALL) -D -m 755 $(TEE_KINGRAY_OUTPUT)/ta/kr_app/*.ta $(TARGET_DIR)/lib/optee_armtz/
	$(INSTALL) -D -m 755 $(TEE_KINGRAY_PRE_BIN)/lib/arm$(TEE_KINGRAY_ARCH)/tee-supplicant $(TARGET_DIR)/usr/bin/
	$(INSTALL) -D -m 755 $(TEE_KINGRAY_PRE_BIN)/lib/arm$(TEE_KINGRAY_ARCH)/libteec.so* $(TARGET_DIR)/usr/lib/
	$(INSTALL) -D -m 755 $(TOPDIR)/package/kingray/tee-kingray/S03anti_copy_board $(TARGET_DIR)/etc/init.d/
endef

$(eval $(generic-package))
