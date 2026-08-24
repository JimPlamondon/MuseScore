import CxxStdlib
import PDFKit
import PreviewProviderCpp
import QuickLookUI
import os.log

// The Command Line Tools toolchain ships no CxxStdlib Swift overlay
// (CxxStdlib.swiftmodule), so overlay conveniences — std.string(String) and
// Sequence conformance for C++ containers — are unavailable; construct
// std.string and Data through the raw imported C++ API instead.
extension std.string {
    init(_ string: String) {
        self.init()
        for byte in string.utf8 {
            self.push_back(CChar(bitPattern: byte))
        }
    }
}

enum PreviewError: Error {
    case previewDataEmpty
    case createPdfFailed
    case getPdfFirstPageFailed
}

class PreviewProvider: QLPreviewProvider, QLPreviewingController {
    let logger = Logger()

    func providePreview(for request: QLFilePreviewRequest) async throws -> QLPreviewReply {
        logger.info("Generating preview for file: \(request.fileURL.path, privacy: .public)")

        await MainActor.run {
            PreviewProviderCxx.initIfNeeded()
        }

        let previewDataCpp = PreviewProviderCxx.getPdfPreviewData(std.string(request.fileURL.path))

        guard !previewDataCpp.empty(), let previewDataPtr = previewDataCpp.__dataUnsafe() else {
            logger.error("Failed to generate preview data")
            throw PreviewError.previewDataEmpty
        }

        let previewData = Data(bytes: previewDataPtr, count: Int(previewDataCpp.size()))

        guard let pdfDocument = PDFDocument(data: previewData) else {
            logger.error("Failed to create PDF document")
            throw PreviewError.createPdfFailed
        }

        guard let firstPage = pdfDocument.page(at: 0) else {
            logger.error("Failed to get first page of PDF document")
            throw PreviewError.getPdfFirstPageFailed
        }

        let pageSize = firstPage.bounds(for: .mediaBox).size

        logger.info(
            "Preview generated successfully with page size: \(pageSize.width, privacy: .public)x\(pageSize.height, privacy: .public)"
        )
        return QLPreviewReply(forPDFWithPageSize: pageSize) { _ in pdfDocument }
    }
}
