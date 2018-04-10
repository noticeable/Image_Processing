CC=	${CROSS_COMPILE}gcc

RGB2GRAY_APP = ./bin/rgb2gray
IMAGERESIZE_APP = ./bin/imageResize
HISTOGRAMEQUALIZATION_APP = ./bin/histogramEqualization
DENOISING_APP = ./bin/denoising
OILPAINTINGEFFECT_APP = ./bin/oilPaintingEffect

DIGITALHALFTONING_APP = ./bin/digitalHalftoning
GEOMETRICWARPING_APP = ./bin/geometricWarping
PANORAMICSTITCHING_APP = ./bin/panoramicStitching
MORPHOLOGY_APP = ./bin/morphology
SOBELEDGEDETECTOR_APP = ./bin/SobelEdgeDetector
ZEROCROSSINGEDGEDETECTOR_APP = ./bin/zeroCrossingEdgeDetector
TEXTURECLASSIFICATION_APP = ./bin/textureClassification


CFLAGS=-g 

COMMON_OBJS =

RGB2GRAY_OBJS = rgb2gray.o
IMAGERESIZE_OBJS = imageResize.o
HISTOGRAMEQUALIZATION_OBJS = histogramEqualization.o
DENOISING_OBJS = denoising.o
OILPAINTINGEFFECT_OBJS = oilPaintingEffect.o

DIGITALHALFTONING_OBJS = digitalHalftoning.o
GEOMETRICWARPING_OBJS = geometricWarping.o
PANORAMICSTITCHING_OBJS = panoramicStitching.o
MORPHOLOGY_OBJS = morphology.o
SOBELEDGEDETECTOR_OBJS = SobelEdgeDetector.o
ZEROCROSSINGEDGEDETECTOR_OBJS = zeroCrossingEdgeDetector.o
TEXTURECLASSIFICATION_OBJS = textureClassification.o



INCLUDE=-I ./inc

#in case you have any particular libraries like the audio library etc.
LIB=		

#loader files, for compilation
LDFLAGS=	-lm 


#/* For compilation*/

#location of source file, related for the path of the makefile 
vpath %.c ./src

all:${RGB2GRAY_APP} ${IMAGERESIZE_APP} ${HISTOGRAMEQUALIZATION_APP} ${DENOISING_APP} ${OILPAINTINGEFFECT_APP} \
	${DIGITALHALFTONING_APP} ${GEOMETRICWARPING_APP} ${PANORAMICSTITCHING_APP} ${MORPHOLOGY_APP} \
	 ${SOBELEDGEDETECTOR_APP} ${ZEROCROSSINGEDGEDETECTOR_APP} ${TEXTURECLASSIFICATION_APP}


rgb2gray:					${RGB2GRAY_APP}
imageResize:				${IMAGERESIZE_APP}
histogramEqualization:		${HISTOGRAMEQUALIZATION_APP}
denoising:					${DENOISING_APP}
oilPaintingEffect:			${OILPAINTINGEFFECT_APP}

digitalHalftoning:					${DIGITALHALFTONING_APP}
geometricWarping:					${GEOMETRICWARPING_APP}
panoramicStitching:					${PANORAMICSTITCHING_APP}
morphology:							${MORPHOLOGY_APP}
SobelEdgeDetctor:					${SOBELEDGEDETECTOR_APP}
zeroCrossingEdgeDetector:			${ZEROCROSSINGEDGEDETECTOR_APP}
textureClassification:				${TEXTURECLASSIFICATION_APP}

.c.o:
	@${CC} ${CFLAGS} ${INCLUDE} -c $< ${LIB} ${LDFLAGS} -o $@
	@echo [CC] $@

${RGB2GRAY_APP}: ${COMMON_OBJS} ${RGB2GRAY_OBJS}
	@${CC} ${COMMON_OBJS} ${RGB2GRAY_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${IMAGERESIZE_APP}: ${COMMON_OBJS} ${IMAGERESIZE_OBJS}
	@${CC} ${COMMON_OBJS} ${IMAGERESIZE_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${HISTOGRAMEQUALIZATION_APP}: ${COMMON_OBJS} ${HISTOGRAMEQUALIZATION_OBJS}
	@${CC} ${COMMON_OBJS} ${HISTOGRAMEQUALIZATION_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${DENOISING_APP}: ${COMMON_OBJS} ${DENOISING_OBJS}
	@${CC} ${COMMON_OBJS} ${DENOISING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${OILPAINTINGEFFECT_APP}: ${COMMON_OBJS} ${OILPAINTINGEFFECT_OBJS}
	@${CC} ${COMMON_OBJS} ${OILPAINTINGEFFECT_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@


${DIGITALHALFTONING_APP}: ${COMMON_OBJS} ${DIGITALHALFTONING_OBJS}
	@${CC} ${COMMON_OBJS} ${DIGITALHALFTONING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${GEOMETRICWARPING_APP}: ${COMMON_OBJS} ${GEOMETRICWARPING_OBJS}
	@${CC} ${COMMON_OBJS} ${GEOMETRICWARPING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${PANORAMICSTITCHING_APP}:${COMMON_OBJS} ${PANORAMICSTITCHING_OBJS}
	@${CC} ${COMMON_OBJS} ${PANORAMICSTITCHING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${MORPHOLOGY_APP}:${COMMON_OBJS} ${MORPHOLOGY_OBJS}
	@${CC} ${COMMON_OBJS} ${MORPHOLOGY_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${SOBELEDGEDETECTOR_APP}:${COMMON_OBJS} ${SOBELEDGEDETECTOR_OBJS}
	@${CC} ${COMMON_OBJS} ${SOBELEDGEDETECTOR_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${ZEROCROSSINGEDGEDETECTOR_APP}:${COMMON_OBJS} ${ZEROCROSSINGEDGEDETECTOR_OBJS}
	@${CC} ${COMMON_OBJS} ${ZEROCROSSINGEDGEDETECTOR_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

${TEXTURECLASSIFICATION_APP}:${COMMON_OBJS} ${TEXTURECLASSIFICATION_OBJS}
	@${CC} ${COMMON_OBJS} ${TEXTURECLASSIFICATION_OBJS} ${CFLAGS} ${INCLUDE} -o $@ ${LIB} ${LDFLAGS}
	@echo [CC] $@

clean:
	find . -type f | xargs touch
	@rm -rf *.o
	@rm -rf ${RGB2GRAY_APP}
	@echo [RM] ${RGB2GRAY_APP}
	@rm -rf ${IMAGERESIZE_APP}
	@echo [RM] ${IMAGERESIZE_APP}
	@rm -rf ${HISTOGRAMEQUALIZATION_APP}
	@echo [RM] ${HISTOGRAMEQUALIZATION_APP}
	@rm -rf ${DENOISING_APP}
	@echo [RM] ${DENOISING_APP}
	@rm -rf ${OILPAINTINGEFFECT_APP}
	@echo [RM] ${OILPAINTINGEFFECT_APP}

	@rm -rf ${DIGITALHALFTONING_APP}
	@echo [RM] ${DIGITALHALFTONING_APP}
	@rm -rf ${GEOMETRICWARPING_APP}
	@echo [RM] ${GEOMETRICWARPING_APP}
	@rm -rf ${PANORAMICSTITCHING_APP}
	@echo [RM] ${PANORAMICSTITCHING_APP}
	@rm -rf ${MORPHOLOGY_APP}
	@echo [RM] ${MORPHOLOGY_APP}
	@rm -rf ${SOBELEDGEDETECTOR_APP}
	@echo [RM] ${SOBELEDGEDETECTOR_APP}
	@rm -rf ${ZEROCROSSINGEDGEDETECTOR_APP}
	@echo [RM] ${ZEROCROSSINGEDGEDETECTOR_APP}
	@rm -rf ${TEXTURECLASSIFICATION_APP}
	@echo [RM] ${TEXTURECLASSIFICATION_APP}