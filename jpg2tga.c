#include <jpgreader.h>
#include <stdio.h>


static TImageInfo imageinfo;


static uint8*
encoderow(uint8* row, uint8* buffer, TImageInfo* info)
{
	uintxx i;

	switch (info->colortype) {
		case IMAGE_GRAY:
			for (i = 0; i < info->sizex; i++) {
				*buffer++ = row[0];
				*buffer++ = row[0];
				*buffer++ = row[0];
				row++;
			}
			return buffer;

		case IMAGE_RGB:
		case IMAGE_YCBCR:
			for (i = 0; i < info->sizex; i++) {
				*buffer++ = row[2];
				*buffer++ = row[1];
				*buffer++ = row[0];
				row += 3;
			}
			return buffer;
	}
	return buffer;
}

static bool
writeheader(FILE* handler, TImageInfo* info, uintxx bpp)
{
	uint8 c;
	uint16 sizex;
	uint16 sizey;
	
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x02; fwrite(&c, 1, 1, handler);
	
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);
	c = 0x00; fwrite(&c, 1, 1, handler);

	sizex = (uint16) info->sizex;
	sizey = (uint16) info->sizey;
	fwrite(&sizex, 1, 2, handler);
	fwrite(&sizey, 1, 2, handler);

	/* image type: rgb(24) or rgba(32) */
	c = bpp;
	fwrite(&c, 1, 1, handler);

	/* pixel origin (top-left) */
	c = 1 << 5;
	fwrite(&c, 1, 1, handler);
	if (ferror(handler)) {
		return 0;
	}
	return 1;
}

static bool
writetga(FILE* handler, TImageInfo* info, uint8* image)
{
	uintxx i;
	uintxx j;
	uintxx rowsize;
	uint8* row;
	uint8* buffer;
	
	j = 24;
	if (writeheader(handler, info, j) == 0) {
		return 0;
	}
	
	j = (j >> 3) * info->sizex;
	buffer = malloc(j);
	if (buffer == NULL) {
		return 0;
	}

	rowsize = imginfo_getrowsize(info);
	row = image;
	for (i = 0; i < info->sizey; i++) {
		encoderow(row, buffer, info);
		fwrite(buffer, 1, j, handler);
		if (ferror(handler)) {
			free(buffer);
			return 0;
		}
		row += rowsize;
	}
	free(buffer);
	return 1;
}

static intxx
rcallback(uint8* buffer, uintxx size, void* user)
{
	intxx r;

	r = fread(buffer, 1, size, (FILE*) user);
	if (r ^ size) {
		if (ferror((FILE*) user)) {
			return -1;
		}
	}
	return r;
}

int
main(int argc, char* argv[])
{
	TJPGReader* jpgr;
	FILE* jpgfile;
	FILE* tgafile;

	if (argc != 3) {
		puts("Usage: thisprogram <jpg file> <target file>");
		return 0;
	}

	jpgfile = fopen(argv[1], "rb");
	tgafile = fopen(argv[2], "wb");
	if (jpgfile == NULL || tgafile == NULL) {
		puts("IO error, failed to open or create file");
		goto L_ERROR2;
	}

	jpgr = jpgr_create(JPGR_IGNOREICCP);
	if (jpgr == NULL) {
		puts("Error: failed to create reader");
		goto L_ERROR2;
	}

	jpgr_setinputfn(jpgr, rcallback, jpgfile);
	if (jpgr_initdecoder(jpgr, &imageinfo)) {
		uint8* image;
		uint8* decodermemory;

		image = malloc(imageinfo.size);
		if (image == NULL) {
			goto L_ERROR2;
		}

		decodermemory = malloc(jpgr->requiredmemory);
		if (decodermemory == NULL) {
			free(image);
			goto L_ERROR2;
		}

		jpgr_setbuffers(jpgr, decodermemory, image);
		if (jpgr_decodeimg(jpgr)) {
			if (writetga(tgafile, &imageinfo, image) == 0) {
				puts("Error: ");
				goto L_ERROR1;
			}
		}
		else {
			puts("Error: failed to decode image");
		}

L_ERROR1:
		free(image);
		free(decodermemory);
	}
	else {
		puts("Error: failed to decode image");
	}
	
	jpgr_destroy(jpgr);
L_ERROR2:
	if (jpgfile)
		fclose(jpgfile);
	if (tgafile)
		fclose(tgafile);
	return 0;
}
