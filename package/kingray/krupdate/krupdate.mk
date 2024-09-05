################################################################################
#
# Kingray krupdate For Linux
#
################################################################################

KRUPDATE_VERSION = 1.0.0
KRUPDATE_SITE = $(TOPDIR)/package/kingray/krupdate
KRUPDATE_SITE_METHOD = local


define KRUPDATE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 755 $(@D)/krupdate $(TARGET_DIR)/usr/bin/
	mkdir $(TARGET_DIR)/etc/update || true
	$(INSTALL) -D -m 755 $(@D)/public_key.pem $(TARGET_DIR)/etc/update/
	$(INSTALL) -D -m 755 $(@D)/S99krupdate $(TARGET_DIR)/etc/init.d/
endef

$(eval $(generic-package))
